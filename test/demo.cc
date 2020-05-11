#include <string>
#include <iostream>

#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"

using namespace Boost::Internal;

int main() {
    const int M = 1024;
    const int N = 512;
    const int K = 256;
    Type index_type = Type::int_scalar(32);
    Type data_type = Type::float_scalar(32);
    
    // index i (from 0 to M)
    Expr dom_i = Dom::make(index_type, 0, M);
    Expr i = Index::make(index_type, "i", dom_i, IndexType::Spatial);

    // index j (from 0 to N)
    Expr dom_j = Dom::make(index_type, 0, N);
    Expr j = Index::make(index_type, "j", dom_j, IndexType::Spatial);


    /* decerating temp1 with index i and j (float temp1[M][N])      */
    Expr expr_temp1 = Var::make(data_type, "temp1", {i, j}, {M, N});
    Expr dec_temp1 = Dec::make(data_type, expr_temp1, false);
    
    /* decerating temp2 with index i and j (float (&temp2)[M][N])   */
    Expr expr_temp2 = Var::make(data_type, "temp1", {i, j}, {M, N});
    Expr dec_temp2 = Dec::make(data_type, expr_temp1, true);

    /* declarating temp3 (float temp3)  */
    Expr expr_temp3 = Var::make(data_type, "temp3", {}, {});
    Expr dec_temp3 = Dec::make(data_type, expr_temp3, false);

    /* 
       - Please wrap the declaration expression with a Stmt if you want to print it 
       - We utilize Move statement to print declaration expression by setting the _src 
            field with an epsilon expression
    */

    /* print expr_temp1 */
    Expr expr_epsilon = Epsilon::make(data_type);   // whatever data_type is ok
    Stmt stmt_temp1 = Move::make(dec_temp1, expr_epsilon, MoveType::MemToMem);

    /* print expr_temp3 and initialize temp3 with 0*/
    Expr expr_init0 = FloatImm::make(data_type, 0);
    Stmt stmt_temp3 = Move::make(dec_temp3, expr_init0, MoveType::MemToMem);

    /**
     * for (int i1 = 0; i1 < M; ++i1) {
     *   for (int j1 = 0; j1 < N; ++j1) {
     *      // some statements
     *   }
     * }
     */
    
    Stmt loop_nest = LoopNest::make({i, j}, {stmt_temp1, stmt_temp3});

    /* 
        IMPORTANT!! 
        All the interface variables MUST be reference declarations
    */
    Expr expr_interfaceA = Var::make(data_type, "interfaceA", {i, j}, {M, N});
    Expr dec_interfaceA = Dec::make(data_type, expr_interfaceA, true);

    Expr expr_interfaceB = Var::make(data_type, "interfaceB", {i, j}, {M, N});
    Expr dec_interfaceB = Dec::make(data_type, expr_interfaceB, true);
    // kernel
    Group kernel = Kernel::make("mytest", {dec_interfaceA}, {dec_interfaceB}, {loop_nest}, KernelType::CPU);

    // visitor
    IRVisitor visitor;
    kernel.visit_group(&visitor);

    // mutator
    IRMutator mutator;
    kernel = mutator.mutate(kernel);

    // printer
    IRPrinter printer;
    std::string code = printer.print(kernel);

    std::cout << "--------------------- demo output -------------------" << std::endl;

    std::cout << code;

    return 0;
}