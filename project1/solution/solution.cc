#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json/json.h"

#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"

#include "y.tab.h"

using namespace Boost::Internal;

void translate(std::string ipath, std::string opath) {

	// read json file and parse it
	std::string name, data_type, kernel;
	std::vector<std::string> ins, outs;
	std::ifstream ifile(ipath, std::ios::binary);
	if (!ifile) return;

	Json::Reader reader;
    Json::Value root;

	if (reader.parse(ifile, root)) {
		name = root["name"].asString();
		for (int i = 0; root["ins"][i].asString() != ""; i++)
			ins.push_back(root["ins"][i].asString());
		for (int i = 0; root["outs"][i].asString() != ""; i++)
			outs.push_back(root["outs"][i].asString());
		data_type = root["data_type"].asString();
		kernel = root["kernel"].asString();
	}

	ifile.close();

	std::ofstream tfile("./temp/case_string", std::ios::out);
	tfile << kernel;
	tfile.close();
	yy_main();


	/////


	/////

	std::ofstream ofile(opath, std::ios::out);
	ofile << "#include \"../run.h\"\n\n";
	ofile.close();
	
}

int main() {

	for (int i = 1; i <= 10; i++) {
		std::string ipath = "./cases/case" + std::to_string(i) + ".json";
		std::string opath = "./kernels/kernel_case" + std::to_string(i) + ".cc";
		translate(ipath, opath);
	}
	return 0;
}

