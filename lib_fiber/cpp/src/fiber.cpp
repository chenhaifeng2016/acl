#include "stdafx.hpp"
#include "fiber/fiber.hpp"

namespace acl {

fiber::fiber(void)
	: f_(NULL)
{
}

fiber::~fiber(void)
{
}

int fiber::get_id(void) const
{
	return f_ ? acl_fiber_id(f_) : -1;
}

int fiber::self(void)
{
	return acl_fiber_self();
}

int fiber::get_errno(void) const
{
	return f_ ? acl_fiber_errno(f_) : -1;
}

void fiber::set_errno(int errnum)
{
	if (f_)
		acl_fiber_set_errno(f_, errnum);
}

void fiber::yield(void)
{
	(void) acl_fiber_yield();
}

void fiber::switch_to_next(void)
{
	acl_fiber_switch();
}

void fiber::ready(fiber& f)
{
	ACL_FIBER *fb = f.get_fiber();

	if (fb)
		acl_fiber_ready(f.get_fiber());
}

void fiber::hook_api(bool on)
{
	acl_fiber_hook_api(on ? 1 : 0);
}

ACL_FIBER *fiber::get_fiber(void) const
{
	return f_;
}

void fiber::start(size_t stack_size /* = 64000 */)
{
	acl_fiber_create(fiber_callback, this, stack_size);
}

void fiber::fiber_callback(ACL_FIBER *f, void *ctx)
{
	fiber* me = (fiber *) ctx;
	me->f_ = f;

	me->run();
}

void fiber::schedule(void)
{
	acl_fiber_schedule();
}

void fiber::stop(void)
{
	acl_fiber_stop();
}

} // namespace acl
