#if defined(WIN32)
# include <Windows.h>
#else
# include "unistd.h"
#endif
#include <stdio.h>
#include "gtest/gtest.h"

#include "csound.hpp"
#include "csPerfThread.hpp"

TEST(PerfThreadsTests, PerfThread) {
    const char  *instrument =
        "instr 1 \n"
        "k1 expon p4, p3, p4*0.001 \n"
        "a1 randi  k1, p5   \n"
        "out  a1   \n"
        "endin \n";

    Csound csound;
    csound.SetOption((char*)"-odac");
    csound.CompileOrc(instrument);
    csound.ReadScore((char*)"i 1 0  3 10000 5000\n");
    csound.Start();

    CsoundPerformanceThread performanceThread1(csound.GetCsound());
    performanceThread1.Play();
    performanceThread1.Join();
    csound.Cleanup();
    csound.Reset();

    CsoundPerformanceThread performanceThread2(csound.GetCsound());
    csound.SetOption((char*)"-odac");
    csound.CompileOrc(instrument);
    csound.ReadScore((char*)"i 1 0  3 10000 5000\n");
    csound.Start();

    performanceThread2.Play();
    performanceThread2.Join();

    csound.Cleanup();
    csound.Reset();
}

TEST(PerfThreadsTests, Record) {
    const char  *instrument =
        "0dbfs = 1.0\n"
        "ksmps = 64\n"
        "instr 1 \n"
        "a1 line 0, p3, 0.5   \n"
        "out  a1   \n"
        "endin \n";

    Csound csound;
    csound.SetOption((char*)"-otestplay.wav");
    csound.SetOption((char*)"-W");
    csound.CompileOrc(instrument);
    csound.ReadScore((char*)"i 1 0  3 0.5 5000\n");
    csound.Start();

    CsoundPerformanceThread performanceThread1(csound.GetCsound());
    performanceThread1.Play();
    //performanceThread1.Record("testrec.wav");

#if !defined(WIN32)
    sleep(1);
#else
    Sleep(1000);
#endif

    //performanceThread1.StopRecord();
    performanceThread1.Join();
    csound.Cleanup();
    csound.Reset();
}
