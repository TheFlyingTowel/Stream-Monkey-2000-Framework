#include "pch.h"


namespace TEST
{
	int add(int a, int b)
	{
		return a + b;
	}

	int sub(int a, int b)
	{
		return a - b;
	}

	int mult(int a, int b)
	{
		return a * b;
	}

	int div(int a, int b)
	{
		return a / b;
	}

	int mod(int a, int b)
	{
		return a % b;
	}

	std::string test()
	{
		return "This is a test string from C++!";
	}

	using namespace pybind11;

	PYBIND11_MODULE(Test, m)
	{
		m.doc() = "This is a test module.";

		m.def("add", &add, "A add function.", arg("a"), arg("b"));
		m.def("sub", &sub, "A sub function.", arg("a"), arg("b"));
		m.def("mult", &mult, "A mult function.", arg("a"), arg("b"));
		m.def("div", &div, "A div function.", arg("a"), arg("b"));
		m.def("mod", &mod, "A mod function.", arg("a"), arg("b"));
		m.def("test", &test, "A test function.");
	}


}

