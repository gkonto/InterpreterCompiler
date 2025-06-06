#ifndef EVALUATOR_HPP_INCLUDED
#define EVALUATOR_HPP_INCLUDED

#include "object.hpp"
#include <memory>

class Node;
class Environment;

__Ptr<EvalObject> Eval(Node *n, __Ptr<Environment> env);

#endif