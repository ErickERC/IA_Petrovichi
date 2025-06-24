#include "../include/dummy_nodes.h"
#include <behaviortree_cpp/bt_factory.h>

int main() {
  // We use the BehaviorTreeFactory to register our custom nodes
  BT::BehaviorTreeFactory factory;
  
  // The recommended way to create a Node is through inheritance.
  factory.registerNodeType<ApproachObject>("ApproachObject");
  
  // Registering a SimpleActionNode using a function pointer.
  // You can use C++11 lambdas or std::bind
  factory.registerSimpleCondition("CheckBattery", [](BT::TreeNode&) { 
    return CheckBattery(); 
  });
  
  //You can also create SimpleActionNodes using methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper", [&gripper](BT::TreeNode&) { 
    return gripper.open(); 
  });
  factory.registerSimpleAction("CloseGripper", [&gripper](BT::TreeNode&) { 
    return gripper.close(); 
  });

  // Trees are created at deployment-time (i.e. at run-time, but only 
  // once at the beginning). 
    
  // IMPORTANT: when the object "tree" goes out of scope, all the 
  // TreeNodes are destroyed
  auto tree = factory.createTreeFromFile("../trees/tutorial01_tree.xml");
  
  // To "execute" a Tree you need to "tick" it.
  // The tick is propagated to the children based on the logic of the tree.
  // In this case, the entire sequence is executed, because all the children
  // of the Sequence return SUCCESS.
  tree.tickWhileRunning();
  
  return 0;
}

/* Expected output:
*
  [ Battery: OK ]
  GripperInterface::open
  ApproachObject: approach_object
  GripperInterface::close
*/