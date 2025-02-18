#include "object.hpp"

static std::string inspectInteger(const EvalObject &o)
{
    return std::to_string(o.getInt());
}

static std::string inspectBoolean(const EvalObject &o)
{
    return std::to_string(o.getBool());
}

static std::string inspectNull()
{
    return "null";
}

static std::string inspectReturn(const EvalObject &o)
{
    return inspect(*o.getObject());
}

static std::string inspectError(const EvalObject &o)
{
    std::string buffer("ERROR: ");
    buffer += o.getString();
    return buffer;
}

std::string inspect(const EvalObject &o)
{
    switch (o.type)
    {
    case ObjType::Integer:
        return inspectInteger(o);
    case ObjType::Boolean:
        return inspectBoolean(o);
    case ObjType::Null:
        return inspectNull();
    case ObjType::Return:
        return inspectReturn(o);
    case ObjType::Error:
        return inspectError(o);
    }
    return "";
}

std::string typeStr(ObjType type)
{
    switch (type)
    {
    case ObjType::Integer:
        return "INTEGER";
    case ObjType::Boolean:
        return "BOOLEAN";
    case ObjType::Null:
        return "NULL";
    case ObjType::Return:
        return "RETURN";
    case ObjType::Error:
        return "ERROR";
    }
    return "";
}