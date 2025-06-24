#include "dummy_nodes.h"
#include <behaviortree_cpp/bt_factory.h>

int main() {
  BT::BehaviorTreeFactory factory;
  
  factory.registerNodeType<SaySomething>("SaySomething");
  factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");
  
  auto tree = factory.createTreeFromFile("../trees/tutorial02_tree.xml");
  
  tree.tickWhileRunning();
  
  return 0;
}

/*  Expected output:
  Robot says: hello
  Robot says: The answer is 42
*/