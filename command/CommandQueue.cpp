#include "CommandQueue.h"

void CommandQueue::push(const Command &command) {
    commandQueue.push(command);
}

Command CommandQueue::pop() {
    Command head = commandQueue.front();
    commandQueue.pop();
    return head;
}

bool CommandQueue::isEmpty() const {
    return commandQueue.empty();
}
