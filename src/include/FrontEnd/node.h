#ifndef _NODE_H_
#define _NODE_H_
#include "global.h"
#include "nodetype.h"
#include "defines.h"
#include <list>
#include <vector>

typedef struct
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} Loc;

class Node
{
  public:
    NodeType type;
    Loc *loc;
    short pass;
    /* parenthesized is set on expressions which were parenthesized
	   in the original source:  e.g., (x+y)*(w+z) would have
	   parenthesized==TRUE on both PLUS nodes, and parenthesized==FALSE
	   on both MULT nodes. */
    bool parenthesized;
    Node() {}
    virtual ~Node() {}
    void setLoc(Loc *loc)
    {
        loc = loc;
    }
    virtual void print() = 0;
    virtual const char *toString() = 0;
};

class primaryNode : public Node
{
  public:
    string name;
    bool isConst;
    primaryNode(string str, Loc *loc) : name(str), isConst(false)
    {
        this->type = primary;
        setLoc(loc);
    }
    ~primaryNode() {}
    void print() { cout << "primNodeType :" << name << endl; }
    const char *toString();
};

class constantNode : public Node
{
  public:
    /* 判断是double/long或者string类型 */
    string name;
    string sval;
    double dval;
    long long llval;
    constantNode(string type, string str, Loc *loc) : name(type), sval(str)
    {
        setLoc(loc);
        this->type = constant;
    }
    constantNode(string type, long long l, Loc *loc) : name(type), llval(l)
    {
        setLoc(loc);
        this->type = constant;
    }
    constantNode(string type, double d, Loc *loc) : name(type), dval(d)
    {
        setLoc(loc);
        this->type = constant;
    }
    ~constantNode() {}
    void print() { cout << "constant :" << type << endl; }
    const char *toString();
};

class identifierNode : public Node
{
  public:
    string name;
    int level;
    int version;
    identifierNode(string name, Loc *loc) : name(name)
    {
        this->type = Id;
        setLoc(loc);
        this->level = Level;
        this->version = current_version[Level];
    }
    ~identifierNode() {}
    void print() {}
    const char *toString()
    {
        string str = "name:" + name;
        str += "  level:" + to_string(level);
        str += "  version:" + to_string(version);
        return str.c_str();
    }
};

class initNode : public Node
{
  public:
    list<Node *> value;
    initNode(Loc *loc)
    {
        this->type = Initializer;
        setLoc(loc);
    }
    ~initNode() {}
    void print() {}
    const char *toString() {}
};

class functionNode : public Node
{
  public:
    functionNode() {}
    ~functionNode() {}
};

class compositeNode : public Node
{
  public:
    compositeNode() {}
    ~compositeNode() {}
};

/* expNode向前声明 */
class expNode;
class adclNode : public Node
{
  public:
    string name;
    int size; // 还未用到
    expNode *dim;
    NodeType valType;
    /* 默认1维 */
    adclNode(NodeType valType, expNode *eNode, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Adcl;
        this->dim = eNode;
        this->valType = valType;
    }
    ~adclNode() {}
    void print() {}
    const char *toString() {}
};

class expNode : public Node
{
  public:
    expNode() {}
    ~expNode() {}
    void print() {}
    const char *toString() {}
};

class declareNode : public Node
{
  public:
    primaryNode *prim;
    list<Node *> *id_List;
    list<Node *> *init_List;
    list<Node *> *adcl_List;
    declareNode(primaryNode *prim, identifierNode *id, adclNode *adcl, initNode *init, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Decl;
        this->prim = prim;
        this->adcl_List->push_back(adcl);
        this->id_List->push_back(id);
        this->init_List->push_back(init);
    }
    declareNode *append(identifierNode *id, adclNode *adcl, initNode *init)
    {
        this->adcl_List->push_back(adcl);
        this->id_List->push_back(id);
        this->init_List->push_back(init);
        return this;
    }
    ~declareNode() {}
    void print() {}
    const char *toString() {}
};
class unaryNode : public Node
{
  public:
    expNode *exp;
    string op;
    unaryNode(string op, expNode *exp, Loc *loc)
    {
        setLoc(loc);
        this->exp = exp;
        this->op = op;
    }
    ~unaryNode() {}
    void print() {}
    const char *toString() {}
};
class binopNode : public Node
{
  public:
    expNode *left;
    expNode *right;
    string op;
    binopNode(expNode *left, string op, expNode *right, Loc *loc)
    {
        this->type = Binop;
        setLoc(loc);
        this->left = left;
        this->right = right;
        op = op;
    }
    ~binopNode() {}
    void print() {}
    const char *toString() {}
};

class ternaryNode : public Node
{
  public:
    expNode *first;
    expNode *second;
    expNode *third;
    ternaryNode(expNode *first, expNode *second, expNode *thrid, Loc *loc)
    {
        setLoc(loc);
        this->type = Ternary;
        this->first = first;
        this->second = second;
        this->third = third;
    }
    ~ternaryNode() {}
    void print() {}
    const char *toString() {}
};

class castNode : public Node
{
  public:
    primaryNode *prim;
    expNode *exp;
    castNode(primaryNode *prim, expNode *exp, Loc *loc)
    {
        setLoc(loc);
        this->type = Cast;
        this->prim = prim;
        this->exp = exp;
    }
    ~castNode() {}
    void print() {}
    const char *toString() {}
};

/* switch() case: */
class statNode;
class caseNode : public Node
{
  public:
    expNode *exp;
    statNode *stmt;
    caseNode(expNode *exp, statNode *stmt, Loc *loc)
    {
        setLoc(loc);
        this->type = Case;
        this->exp = exp;
        this->stmt = stmt;
    }
    ~caseNode() {}
    void print() {}
    const char *toString() {}
};

class defaultNode : public Node
{
  public:
    statNode *stmt;
    defaultNode(statNode *stmt, Loc *loc)
    {
        setLoc(loc);
        this->type = Default;
        this->stmt = stmt;
    }
    ~defaultNode() {}
    void print() {}
    const char *toString() {}
};

class continueNode : public Node
{
  public:
    continueNode(Loc *loc)
    {
        this->setLoc(loc);
        this->type = Continue;
    }
    ~continueNode() {}
    void print() {}
    const char *toString() {}
};

class breakNode : public Node
{
  public:
    breakNode(Loc *loc)
    {
        this->setLoc(loc);
        this->type = Break;
    }
    ~breakNode() {}
    void print() {}
    const char *toString() {}
};

class returnNode : public Node
{
  public:
    expNode *exp;
    returnNode(expNode *exp, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Return;
        this->exp = exp;
    }
    ~returnNode() {}
    void print() {}
    const char *toString() {}
};

class ifNode : public Node
{
  public:
    expNode *exp;
    Node *stmt;
    ifNode(expNode *exp, Node *stmt, Loc *loc)
    {
        this->setLoc(loc);
        this->type = If;
        this->exp = exp;
        this->stmt = stmt;
    }
    ~ifNode() {}
    void print() {}
    const char *toString() {}
};

class ifElseNode : public Node
{
  public:
    expNode *exp;
    Node *stmt1;
    Node *stmt2;
    ifElseNode(expNode *exp, Node *stmt1, Node *stmt2, Loc *loc)
    {
        this->setLoc(loc);
        this->type = IfElse;
        this->exp = exp;
        this->stmt1 = stmt1;
        this->stmt2 = stmt2;
    }
    ~ifElseNode() {}
    void print() {}
    const char *toString() {}
};

class switchNode : public Node
{
  public:
    expNode *exp;
    statNode *stat;
    switchNode(expNode *exp, statNode *stat, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Switch;
        this->exp = exp;
        this->stat = stat;
    }
    ~switchNode() {}
    void print() {}
    const char *toString() {}
};
class pipelineNode : public Node
{
  public:
    list<Node *> *split_pipe;
    pipelineNode(list<Node *> *split_pipe, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Pipeline;
        this->split_pipe = split_pipe;
    }
    ~pipelineNode() {}
    void print() {}
    const char *toString(){};
};

class roundrobinNode : public Node
{
  public:
    list<Node *> *arg_list;
    roundrobinNode(list<Node *> *arg_list, Loc *loc)
    {
        this->setLoc(loc);
        this->type = RoundRobin;
        this->arg_list = arg_list;
    }
    ~roundrobinNode() {}
    void print() {}
    const char *toString(){};
};

class duplicateNode : public Node
{
  public:
    expNode *exp;
    duplicateNode(expNode *exp, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Duplicate;
        this->exp = exp;
    }
    ~duplicateNode() {}
    void print() {}
    const char *toString(){};
};

class splitNode : public Node
{
  public:
    /* 标识split节点的类型 roundrobin/duplicate*/
    string name;
    splitNode *spt;
    duplicateNode *dup;
    splitNode(string name, splitNode *spt, Loc *loc)
    {
        this->type = Split;
        this->setLoc(loc);
        this->spt = spt;
        this->name = name;
    }
    splitNode(string name, duplicateNode *dup, Loc *loc)
    {
        this->type = Split;
        this->setLoc(loc);
        this->dup = dup;
        this->name = name;
    }
    ~splitNode() {}
    void print() {}
    const char *toString(){};
};

class joinNode : public Node
{
  public:
    roundrobinNode *rdb;
    joinNode(roundrobinNode *rdb, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Join;
        this->rdb = rdb;
    }
    ~joinNode() {}
    void print() {}
    const char *toString(){};
};

class splitjoinNode : public Node
{
  public:
    splitNode *split;
    joinNode *join;
    list<Node *> *stmt_list;
    list<Node *> *split_pipe;
    splitjoinNode(splitNode *split, list<Node *> *stmt_list, list<Node *> *split_pipe, joinNode *join, Loc *loc)
    {
        this->setLoc(loc);
        this->type = SplitJoin;
        this->split = split;
        this->join = join;
        this->stmt_list = stmt_list;
        this->split_pipe = split_pipe;
    }
    ~splitjoinNode() {}
    void print() {}
    const char *toString(){};
};

class addNode : public Node
{
  public:
    /* content可以为pipeline，splitjoin或者composite调用 */
    Node *content;
    addNode(Node *content, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Add;
        this->content = content;
    }
    ~addNode() {}
    void print() {}
    const char *toString(){};
};

class slidingNode : public Node
{
  public:
    list<Node *> *arg_list;
    slidingNode(list<Node *> *arg_list, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Sliding;
        this->arg_list = arg_list;
    }
    ~slidingNode() {}
    void print() {}
    const char *toString(){};
};

class tumblingNode : public Node
{
  public:
    list<Node *> *arg_list;
    tumblingNode(list<Node *> *arg_list, Loc *loc)
    {
        this->setLoc(loc);
        this->type = Tumbling;
        this->arg_list = arg_list;
    }
    ~tumblingNode() {}
    void print() {}
    const char *toString(){};
};

class compositeCallNode : public Node
{
  public:
    string name;
    list<Node *> *arg_List;
    compositeCallNode(string name, list<Node *> *arg_List, Loc *loc)
    {
        this->setLoc(loc);
        this->type = CompositeCall;
        this->name = name;
        this->arg_List = arg_List;
    }
    ~compositeCallNode() {}
    void print() {}
    const char *toString(){};
};
#endif