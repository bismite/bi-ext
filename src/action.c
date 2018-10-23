#include <bi/ext/action.h>
#include <bi/context.h>
#include <stdlib.h>

typedef struct {
  BiAction* action;
} ActionDoer;

void bi_action_start(BiNode *node, BiAction *action,double now)
{
  action->_start(node,action,now);
}

void bi_action_update(BiNode *node, BiAction *action, double rate)
{
  action->_update(node,action,rate);
}

static bool do_actions(BiContext* context,BiNode* node,double now,BiTimer* timer)
{
  ActionDoer* doer = timer->userdata;
  BiAction* a = doer->action;
  double rate = (now - a->start_at) / a->duration;
  if(rate<0.0) rate = 0.0;
  if(rate>1.0) rate = 1.0;
  bi_action_update(node,a,rate);
  return true;
}

void bi_add_action(BiContext* context,BiNode* node,BiAction* action)
{
  action->_timer = malloc(sizeof(BiTimer));
  bi_timer_init(action->_timer, node, do_actions, 0, -1, NULL);
  ActionDoer* doer = malloc(sizeof(ActionDoer));
  doer->action = action;
  action->_timer->userdata = doer;
  bi_add_timer(context,action->_timer);
}

void bi_remove_action(BiContext* context,BiNode* node,BiAction* action)
{
  bi_remove_timer(context,action->_timer);
  free(action->_timer->userdata);
  action->_timer = NULL;
}
