#pragma once

#include <boost/logging/format_fwd.hpp>
#include "global.h"

#include <QDebug>

// Step 1: Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> )

#ifndef BOOST_LOG_COMPILE_FAST
#include <boost/logging/format.hpp>
#include <boost/logging/writer/ts_write.hpp>
#endif

// Step 3 : Specify your logging class(es)
typedef boost::logging::logger_format_write< > log_type;

// Step 4: declare which filters and loggers you'll use
BOOST_DECLARE_LOG_FILTER(g_l_filter, boost::logging::level::holder)
BOOST_DECLARE_LOG(g_l, log_type)

// Step 5: define the macros through which you'll log
#define LDEBUG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), debug ) << "[DEBUG] "
// #define LDEBUG_ qDebug()
#define LERROR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), error ) << "[ERROR] "
#define LWARN_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), warning ) << "[WARNING] "
#define LPLUGIN_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info ) << "[PLUGIN] "
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info ) << "[INFO] "
#define LSEPARATOR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_filter(), info ) << "-- Logging started --"

#define QSTR(string) string.toStdString().c_str()

void WRSYNC_API init_logs();
