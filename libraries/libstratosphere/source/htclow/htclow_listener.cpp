/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "htclow_listener.hpp"

namespace ams::htclow {

    namespace {

        constexpr inline size_t ThreadStackSize = 4_KB;

    }

    Listener::Listener(mem::StandardAllocator *allocator, mux::Mux *mux, ctrl::HtcctrlService *ctrl_srv, Worker *worker)
        : m_thread_stack_size(ThreadStackSize), m_allocator(allocator), m_mux(mux), m_service(ctrl_srv), m_worker(worker), m_event(os::EventClearMode_ManualClear), m_driver(nullptr), m_thread_running(false), m_cancelled(false)
    {
        /* Allocate stack. */
        m_listen_thread_stack = m_allocator->Allocate(m_thread_stack_size, os::ThreadStackAlignment);
    }

    void Listener::Start(driver::IDriver *driver) {
        /* Check pre-conditions. */
        AMS_ASSERT(!m_thread_running);

        /* Create the thread. */
        R_ABORT_UNLESS(os::CreateThread(std::addressof(m_listen_thread), ListenThreadEntry, this, m_listen_thread_stack, ThreadStackSize, AMS_GET_SYSTEM_THREAD_PRIORITY(htc, HtclowListen)));

        /* Set the thread name. */
        os::SetThreadNamePointer(std::addressof(m_listen_thread), AMS_GET_SYSTEM_THREAD_NAME(htc, HtclowListen));

        /* Set our driver. */
        m_driver = driver;

        /* Set state. */
        m_thread_running = true;
        m_cancelled      = false;

        /* Clear our event. */
        m_event.Clear();

        /* Start the thread. */
        os::StartThread(std::addressof(m_listen_thread));
    }

    void Listener::ListenThread() {
        /* TODO */
        AMS_ABORT("Listener::ListenThread");
    }

}