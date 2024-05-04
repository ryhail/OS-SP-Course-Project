
#ifndef COURSE_COMMANDQUEUE_H
#define COURSE_COMMANDQUEUE_H

#include <queue>
#include "Command.h"

class CommandQueue {
public:
    void                    push(const Command& command);;
    Command                 pop();
    [[nodiscard]] bool      isEmpty() const;
private:
    std::queue<Command> commandQueue;
};

#endif //COURSE_COMMANDQUEUE_H
