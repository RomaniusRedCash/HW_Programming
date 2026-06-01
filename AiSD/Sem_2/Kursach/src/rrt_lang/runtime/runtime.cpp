#include "runtime.h"

void runtime::run(const std::vector<std::unique_ptr<command>>& commands, world &wrd) {
    for (const std::unique_ptr<command>& cmnd : commands) {
        cmnd->do_work(wrd);
    }
}
