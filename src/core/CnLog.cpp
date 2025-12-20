#include "CnLog.h"

#include <CnStd_string.h>
#include <CnStd_sstream.h>

namespace Cn {

namespace Log { // namespace Cn::Log

Strings::Strings() :
    Log_Error          (CnSTR("Error")),
    Log_Warning        (CnSTR("Warning")),
    Log_Info           (CnSTR("Info")),
    Log_Trace          (CnSTR("Trace")),
    Log_TraceDetails   (CnSTR("TraceDetails")),
    Log_CtorDtor       (CnSTR("CtorDtor")),
    Log_EntryExit      (CnSTR("EntryExit")),
    Log_ThreadStartStop(CnSTR("ThreadStartStop")),
    Log_Connection     (CnSTR("Connection")),
    Log_Item           (CnSTR("Item")),
    Log_Message        (CnSTR("Message")),
    Log_Protocol       (CnSTR("Protocol")),
    Log_Receive        (CnSTR("Receive")),
    Log_Send           (CnSTR("Send"))
{
}

const Strings &Strings::instance()
{
    static const Strings s;
    return s;
}

} // namespace Cn::Log

const CnChar *toString(LogFlag flag)
{
    const Log::Strings &s = Log::Strings::instance();
    switch (flag)
    {
    case Log_Error          : return s.Log_Error          .data();
    case Log_Warning        : return s.Log_Warning        .data();
    case Log_Info           : return s.Log_Info           .data();
    case Log_Trace          : return s.Log_Trace          .data();
    case Log_TraceDetails   : return s.Log_TraceDetails   .data();
    case Log_CtorDtor       : return s.Log_CtorDtor       .data();
    case Log_EntryExit      : return s.Log_EntryExit      .data();
    case Log_ThreadStartStop: return s.Log_ThreadStartStop.data();
    case Log_Connection     : return s.Log_Connection     .data();
    case Log_Item           : return s.Log_Item           .data();
    case Log_Message        : return s.Log_Message        .data();
    case Log_Protocol       : return s.Log_Protocol       .data();
    case Log_Receive        : return s.Log_Receive        .data();
    case Log_Send           : return s.Log_Send           .data();
    }
    return nullptr;
}

LogFlag toLogFlag(const CnChar *sflag)
{
    const Log::Strings &s = Log::Strings::instance();
    if (sflag == s.Log_Error          ) return Log_Error          ;
    if (sflag == s.Log_Warning        ) return Log_Warning        ;
    if (sflag == s.Log_Info           ) return Log_Info           ;
    if (sflag == s.Log_Trace          ) return Log_Trace          ;
    if (sflag == s.Log_TraceDetails   ) return Log_TraceDetails   ;
    if (sflag == s.Log_CtorDtor       ) return Log_CtorDtor       ;
    if (sflag == s.Log_EntryExit      ) return Log_EntryExit      ;
    if (sflag == s.Log_ThreadStartStop) return Log_ThreadStartStop;
    if (sflag == s.Log_Connection     ) return Log_Connection     ;
    if (sflag == s.Log_Item           ) return Log_Item           ;
    if (sflag == s.Log_Message        ) return Log_Message        ;
    if (sflag == s.Log_Protocol       ) return Log_Protocol       ;
    if (sflag == s.Log_Receive        ) return Log_Receive        ;
    if (sflag == s.Log_Send           ) return Log_Send           ;
    return static_cast<LogFlag>(0);
}

CnString toString(LogFlags flags)
{
    CnList<const CnChar *> ls;
    const Log::Strings &s = Log::Strings::instance();
    if (flags & Log_Error          ) ls.push_back(s.Log_Error          .data());
    if (flags & Log_Warning        ) ls.push_back(s.Log_Warning        .data());
    if (flags & Log_Info           ) ls.push_back(s.Log_Info           .data());
    if (flags & Log_Trace          ) ls.push_back(s.Log_Trace          .data());
    if (flags & Log_TraceDetails   ) ls.push_back(s.Log_TraceDetails   .data());
    if (flags & Log_CtorDtor       ) ls.push_back(s.Log_CtorDtor       .data());
    if (flags & Log_EntryExit      ) ls.push_back(s.Log_EntryExit      .data());
    if (flags & Log_ThreadStartStop) ls.push_back(s.Log_ThreadStartStop.data());
    if (flags & Log_Connection     ) ls.push_back(s.Log_Connection     .data());
    if (flags & Log_Item           ) ls.push_back(s.Log_Item           .data());
    if (flags & Log_Message        ) ls.push_back(s.Log_Message        .data());
    if (flags & Log_Protocol       ) ls.push_back(s.Log_Protocol       .data());
    if (flags & Log_Receive        ) ls.push_back(s.Log_Receive        .data());
    if (flags & Log_Send           ) ls.push_back(s.Log_Send           .data());

    if (ls.empty()) 
        return CnString();  // Prevent undefined behavior

    CnStd::ostringstream oss;
    auto it = ls.begin();
    oss << *it; // Safe because we checked for emptiness

    for (++it; it != ls.end(); ++it) {
        oss << CnCHR('|') << *it;
    }

    return CnString(oss.str().data());
}

LogFlags toLogFlags(const CnString &sflags)
{
    static std::unordered_map<CnStd::string, LogFlag> flagMap = 
    {
        { Log::Strings::instance().Log_Error          .data() , Cn::Log_Error           },
        { Log::Strings::instance().Log_Warning        .data() , Cn::Log_Warning         },
        { Log::Strings::instance().Log_Info           .data() , Cn::Log_Info            },
        { Log::Strings::instance().Log_Trace          .data() , Cn::Log_Trace           },
        { Log::Strings::instance().Log_TraceDetails   .data() , Cn::Log_TraceDetails    },
        { Log::Strings::instance().Log_CtorDtor       .data() , Cn::Log_CtorDtor        },
        { Log::Strings::instance().Log_EntryExit      .data() , Cn::Log_EntryExit       },
        { Log::Strings::instance().Log_ThreadStartStop.data() , Cn::Log_ThreadStartStop },
        { Log::Strings::instance().Log_Connection     .data() , Cn::Log_Connection      },
        { Log::Strings::instance().Log_Item           .data() , Cn::Log_Item            },
        { Log::Strings::instance().Log_Message        .data() , Cn::Log_Message         },
        { Log::Strings::instance().Log_Protocol       .data() , Cn::Log_Protocol        },
        { Log::Strings::instance().Log_Receive        .data() , Cn::Log_Receive         },
        { Log::Strings::instance().Log_Send           .data() , Cn::Log_Send            }
    };
    CnStd::string sflagsStd = sflags.toStdString();

    CnStd::istringstream stream(sflagsStd);
    CnStd::string flag;
    LogFlags flags = 0;
    while (std::getline(stream, flag, CnCHR('|'))) 
    {
        flag.erase(0, flag.find_first_not_of(CnCHR(' ')));
        flag.erase(flag.find_last_not_of(CnCHR(' ')) + 1);
        auto it = flagMap.find(flag);
        if (it != flagMap.end()) 
            flags |= it->second;
    }
    return flags;   
}

} // namespace Cn
