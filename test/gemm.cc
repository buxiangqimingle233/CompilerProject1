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

    // index i
    Expr dom_i = Dom::make(index_type, 0, M);
    Expr i = Index::make(index_type, "i", dom_i, IndexType::Spatial);

    // index j
    Expr dom_j = Dom::make(index_type, 0, N);
    Expr j = Index::make(index_type, "j", dom_j, IndexType::Spatial);

    // index k
    Expr dom_k = Dom::make(index_type, 0, K);
    Expr k = Index::make(index_type, "k", dom_k, IndexType::Reduce);

    // A
    Expr expr_A = Var::make(data_type, "A", {i, k}, {M, K});
    Expr dec_A = Dec::make(data_type, expr_A, true);

    // B
    Expr expr_B = Var::make(data_type, "B", {k, j}, {K, N});
    Expr dec_B = Dec::make(data_type, expr_B, true);

    // C
    Expr expr_C = Var::make(data_type, "C", {i, j}, {M, N});
    Expr dec_C = Dec::make(data_type, expr_C, true);

    // test for declaration
    Expr expr_D = Var::make(data_type, "D", {}, {});
    Expr dec_D = Dec::make(data_type, expr_D, false);
    Expr epsilon = Epsilon::make(data_type);
    Stmt dec_stmt = Move::make(expr_D, epsilon, MoveType::MemToMem);

    // main stmt
    Stmt main_stmt = Move::make(
        expr_C,
        Binary::make(data_type, BinaryOpType::Add, expr_C,
            Binary::make(data_type, BinaryOpType::Mul, expr_A, expr_B)),
        MoveType::MemToMem
    );

    // loop nest
    Stmt loop_nest = LoopNest::make({i, j, k}, {main_stmt});

    // kernel
    Group kernel = Kernel::make("simple_gemm", {dec_A, dec_B}, {dec_C}, {dec_stmt, loop_nest}, KernelType::CPU);

    // visitor
    IRVisitor visitor;
    kernel.visit_group(&visitor);

    // mutator
    IRMutator mutator;
    kernel = mutator.mutate(kernel);

    // printer
    IRPrinter printer;
    std::string code = printer.print(kernel);

    std::cout << code;

    std::cout << "Success!\n";
    return 0;
}