#include <bi/ext/action.h>

typedef struct {
  BiAction* action;
} BiActionRepeat;

static void bi_action_repeat_start(BiNode* node, BiAction* action,double now)
{
  action->start_at = now;
  BiActionRepeat* rep = action->action_data;
  bi_action_start(node,rep->action,now);
}

static bool bi_action_repeat_update(BiNode* node, BiAction* action, double rate)
{
  BiActionRepeat* rep = action->action_data;

  if(rate >= 1.0) {
    // finish
    bi_action_update( node, rep->action, 1.0 );
    // restart
    action->start_at += rep->action->duration;
    bi_action_start(node,rep->action,action->start_at);
    rate = 0;
  }

  bi_action_update( node, rep->action, rate );
  return true;
}

void bi_action_repeat_init(BiAction* action,BiAction* target)
{
  BiActionRepeat* rep = malloc(sizeof(BiActionRepeat));
  rep->action = target;

  action->_update = bi_action_repeat_update;
  action->_start = bi_action_repeat_start;
  action->duration = target->duration;
  action->action_data = rep;
}
