#include "VMFunction.h"


VMFunction::VMFunction() : VMFunction("", 0)
{

}

VMFunction::VMFunction(std::string name, int parameter_count) : m_name(name), m_parameter_count(parameter_count)
{
}


VMFunction::~VMFunction()
{
}
