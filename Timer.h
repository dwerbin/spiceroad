#include <chrono>

namespace sr {

template <class DT = std::chrono::milliseconds,
          class ClockT = std::chrono::steady_clock>
class Timer {
   public:
    Timer() : start(ClockT::now()) {}
    void restart() { start = ClockT::now(); }
    template <class T = DT>
    auto duration() const {
        return std::chrono::duration_cast<T>(ClockT::now() - start);
    }

   protected:
    typename ClockT::time_point start;
};

}  // namespace sr
