#include <bi/ext/action.h>
#include <stdlib.h>

typedef struct {
  int from_x;
  int from_y;
  int to_x;
  int to_y;
} BiActionMoveTo;

static bool bi_action_move_to_update(BiNode* node, BiAction* action, double rate)
{
  BiActionMoveTo* move_to = action->action_data;
  int x = move_to->from_x + (move_to->to_x - move_to->from_x) * rate;
  int y = move_to->from_y + (move_to->to_y - move_to->from_y) * rate;
  bi_node_set_position(node,x,y);
  return true;
}

static void bi_action_move_to_start(BiNode* node, BiAction* action,double now)
{
  BiActionMoveTo* move_to = action->action_data;
  action->start_at = now;
  move_to->from_x = node->x;
  move_to->from_y = node->y;
}

void bi_action_move_to_init(BiAction* action,double duration,int x,int y)
{
  action->_update = bi_action_move_to_update;
  action->_start = bi_action_move_to_start;
  action->duration = duration;
  BiActionMoveTo* move_to = malloc(sizeof(BiActionMoveTo));
  move_to->to_x = x;
  move_to->to_y = y;
  action->action_data = move_to;
}
