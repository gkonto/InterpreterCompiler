#ifndef ENVIRONMENT_HPP_INCLUDED
#define ENVIRONMENT_HPP_INCLUDED

#include <map>
#include <string>
#include <memory>
#include "macros.hpp"

struct EvalObject;

class Environment
{
public:
    Environment() = default;
    Environment(__Ptr<Environment> env)
        : outer_(env) {}

    __Ptr<EvalObject> get(const std::string &name) const;
    void set(const std::string &name, __Ptr<EvalObject> val);

private:
    std::map<std::string, __Ptr<EvalObject>> repo_;
    __Ptr<Environment> outer_ = nullptr;
};

#endif