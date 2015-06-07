#include"stdafx.hpp"
#include"AoObject.hpp"


using namespace ao;
AO_ROOT_CLASS_IMPLEMENT(AoObject);


AoObject::AoObject() : name(_T("AoObject"))
{

}


AoObject::AoObject(AOLPSTR _name) : name(_name)
{

}


AoObject::AoObject(const string& _name) : name(_name)
{

}


AoObject::~AoObject()
{

}