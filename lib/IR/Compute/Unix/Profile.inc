//===- Profile.inc --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Profile.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/Support/PerfEvent.h>
#include <unistd.h>
#include <cassert>
#include <iostream>

//TODO: Find a way to choose what interface ONNI should choose for profiling.
//#if defined(HAVE_LINUX_PERF_EVENT_H)
#include <linux/perf_event.h>
#include <sys/ioctl.h>
//#if defined(HAVE_ASM_UNISTD_H)
#include <asm/unistd.h>
//#endif
//#endif

using namespace onnc;

char Profile::ID = 0;

//===----------------------------------------------------------------------===//
// ProfileImpl
//===----------------------------------------------------------------------===//
class ProfileImpl
{
public:
  ProfileImpl() {
  }
  ~ProfileImpl() {
//#if defined(HAVE_LINUX_PERF_EVENT_H)
    close(m_Fd);
//#endif
  }

  Profile::Interval getCounter() {
//#if defined(HAVE_LINUX_PERF_EVENT_H)
    unsigned long long counter;
    read(m_Fd, &counter, sizeof(unsigned long long));
    return counter;
//#else
//    return 0;
//#endif
     return -1;
  }
  void init(enum PerfEvent pEvent) {
//#if defined(HAVE_LINUX_PERF_EVENT_H)

    /* store the perf event numbers with the same order of skypat:Perf_event_name */
    static const decltype(perf_event_attr::config)
        event_list[] = { 
            PERF_COUNT_HW_CPU_CYCLES, PERF_COUNT_HW_INSTRUCTIONS,
            PERF_COUNT_HW_CACHE_REFERENCES, PERF_COUNT_HW_CACHE_MISSES,
            PERF_COUNT_HW_BRANCH_INSTRUCTIONS, PERF_COUNT_HW_BRANCH_MISSES,
            PERF_COUNT_HW_BUS_CYCLES, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,
            PERF_COUNT_HW_STALLED_CYCLES_BACKEND, PERF_COUNT_HW_REF_CPU_CYCLES,
            PERF_COUNT_SW_CPU_CLOCK, PERF_COUNT_SW_TASK_CLOCK,
            PERF_COUNT_SW_PAGE_FAULTS, PERF_COUNT_SW_CONTEXT_SWITCHES,
            PERF_COUNT_SW_CPU_MIGRATIONS, PERF_COUNT_SW_PAGE_FAULTS_MIN,
            PERF_COUNT_SW_PAGE_FAULTS_MAJ, PERF_COUNT_SW_ALIGNMENT_FAULTS,
            PERF_COUNT_SW_EMULATION_FAULTS,
//#ifdef PERF_COUNT_SW_DUMMY
//            PERF_COUNT_SW_DUMMY
//#else
            0   
//#endif
    };  

    struct perf_event_attr attr;
    
    memset(&attr, 0, sizeof(attr));

    attr.inherit = 1;
    attr.disabled = 1;

    attr.config = event_list[pEvent];


    if(pEvent < PerfEvent::CPU_CLOCK)
        attr.type = PERF_TYPE_HARDWARE;
    else
        attr.type = PERF_TYPE_SOFTWARE;

    attr.size = sizeof(attr);

    m_Fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
//#endif
  }

  void start() {
//#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_ENABLE);
//#endif
    m_Start = getCounter();
    assert(-1 != m_Start && "fail to get performance counters");
  }

  void stop() {
//#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_DISABLE);
//#endif
    m_End = getCounter();
    assert(-1 != m_End && "fail to get performance counters");
  }

  Profile::Interval getValue() const {
    return (m_End - m_Start);
  }
private:
  Profile::Interval m_Start;
  Profile::Interval m_End;

  static long g_ClkTick;

  int m_Fd;
};


long ProfileImpl::g_ClkTick = -1;

static ManagedStatic<ProfileImpl> g_Profile;
//===----------------------------------------------------------------------===//
// Profile
//===----------------------------------------------------------------------===//
Profile::Profile()
  : ComputeOperator("Profile", ID) {
  g_Profile->init(PerfEvent::CPU_CYCLES);
}

Profile::Profile(const Profile& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

Profile::~Profile() {
}

void Profile::start() {
  m_bIsActive = true;
  g_Profile->start();
}

void Profile::stop() {
  m_bIsActive = false;
  m_Interval = g_Profile->getValue();
  std::cout << "getValue = " << m_Interval << "\n";
}

bool Profile::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
