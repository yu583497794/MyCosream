#include "staticStreamGragh.h"
static StaticStreamGraph *ssg = NULL;

//只针对composite节点
void GraphToOperators(compositeNode *composite, compositeNode *oldComposite)
{
    /* 获取compositebody内的statementNode */
    list<Node *> list = *(composite->body->bodystmt_List);
    cout<<"list.size()= "<<list.size()<<endl;
    for (auto it : (list))
    {
        cout<<it->type<<endl;
        switch (it->type)
        {
        case Operator_:
            cout <<((operatorNode*)it)->operName<< "Operator_" << endl;
            break;
        case CompositeCall:
            cout << "compositeCall" << endl;
            break;
        case SplitJoin:
            cout << "SplitJoin" << endl;
            break;
        case Pipeline:
            cout << "Pipeline" << endl;
            break;
        }
    }
    

    return;
}

StaticStreamGraph *AST2FlatStaticStreamGraph(compositeNode *mainComposite)
{
    ssg = new StaticStreamGraph();
    GraphToOperators(mainComposite, mainComposite);
    return ssg;
}
