#include "TokenList.hpp"

void TokenList::push(Node& node) {

    auto temp{std::make_unique<Node>(std::move(node))};
    if(head) {
        temp->next = std::move(head);
        head = std::move(temp);
    }
    else {
        head = std::move(temp);    
    }
}