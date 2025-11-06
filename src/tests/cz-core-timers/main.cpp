#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZLog.h>
#include <chrono>

using namespace CZ;

static int RunningTimers { 5 };

int main()
{
    setenv("CZ_CORE_LOG_LEVEL", "4", 1);

    auto core { CZCore::GetOrMake() };
    const auto start { std::chrono::steady_clock::now() };

    /* TIMER 1 */

    CZTimer timer1 {[start](CZTimer *t){
        const auto end { std::chrono::steady_clock::now() };
        CZLog(CZInfo, "Timer 1 finished. Timeout: {}ms finished at: {}",
              t->timeoutMs(),
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        RunningTimers--;
    }};
    timer1.start(1000);
    CZLog(CZInfo, "Timer 1 (normal) started timeout {}ms", timer1.timeoutMs());

    /* TIMER 2 */

    CZTimer timer2 {[start](CZTimer *t){
        const auto end { std::chrono::steady_clock::now() };
        CZLog(CZInfo, "Timer 2 finished. Timeout: {}ms finished at: {}",
              t->timeoutMs(),
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        RunningTimers--;
    }};
    timer2.start(0);
    CZLog(CZInfo, "Timer 2 (normal) started timeout {}ms", timer2.timeoutMs());

    /* TIMER 3 */

    CZTimer timer3 {[start](CZTimer *t){
        const auto end { std::chrono::steady_clock::now() };
        CZLog(CZInfo, "Timer 3 finished. Timeout: {}ms finished at: {}",
              t->timeoutMs(),
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        RunningTimers--;
    }};
    timer3.start(500);
    CZLog(CZInfo, "Timer 3 (normal) started timeout {}ms", timer3.timeoutMs());

    /* TIMER 4 */

    CZTimer::OneShot(2000, [start](CZTimer *t){
        const auto end { std::chrono::steady_clock::now() };
        CZLog(CZInfo, "Timer 4 finished. Timeout: {}ms finished at: {}",
              t->timeoutMs(),
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        RunningTimers--;
    });
    CZLog(CZInfo, "Timer 4 (oneshot) started timeout {}ms", 2000);

    /* TIMER 5 */

    CZTimer timer5 {[start](CZTimer *t){
        const auto end { std::chrono::steady_clock::now() };
        CZLog(CZInfo, "Timer 5 finished. Timeout: {}ms finished at: {}",
              t->timeoutMs(),
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        RunningTimers--;
    }};
    timer5.start(3000);
    CZLog(CZInfo, "Timer 5 (normal) started timeout {}ms", timer5.timeoutMs());

    while (RunningTimers > 0)
        core->dispatch(-1);

    return 0;
}
