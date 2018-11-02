#include <bi/ext/action.h>
#include <stdlib.h>

static bool bi_action_call_function_update(BiNode* node, BiAction* action, double rate)
{
  return true;
}

static void bi_action_call_function_start(BiNode* node, BiAction* action,double now)
{
  action->start_at = now;
  BiActionCallFunction* call = action->action_data;
  call->function(action,call->payload);
}

void bi_action_call_function_init(BiAction* action, bi_action_call_function function,void* payload)
{
  BiActionCallFunction* call = action->action_data;
  call->function = function;
  call->payload = payload;
  action->update = bi_action_call_function_update;
  action->start = bi_action_call_function_start;
  action->duration = 0;
  action->on_finish = NULL;
}
