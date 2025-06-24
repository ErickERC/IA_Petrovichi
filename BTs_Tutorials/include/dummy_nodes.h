#ifndef DUMMY_NODES_H
#define DUMMY_NODES_H

#include <iostream>
#include <string>
#include <cstdio>
#include <chrono>
#include <thread>
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/action_node.h>
#include <behaviortree_cpp/basic_types.h>

// ===== Tutorial 01 : Your first Behavior Tree =====

// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode {
  public:
    ApproachObject(const std::string& name) : 
        BT::SyncActionNode(name, {}) {}

    // You must override the virtual function tick()
    BT::NodeStatus tick() override {
      std::cout << "ApproachObject: " << this->name() << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
  };

// Simple function that return a NodeStatus
inline BT::NodeStatus CheckBattery() {
  std::cout << "[ Battery: OK ]" << std::endl;
  return BT::NodeStatus::SUCCESS;
}

// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface {
public:
  GripperInterface() : _open(true) {}
  
  BT::NodeStatus open() {
    _open = true;
    std::cout << "GripperInterface::open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close() {
    _open = false;
    std::cout << "GripperInterface::close" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool _open; // shared information
};


// ===== Tutorial 02 : Blackboard and ports =====

// SyncActionNode (synchronous action) with an input port.
class SaySomething : public BT::SyncActionNode {
  public:
    // If your Node has ports, you must use this constructor signature 
    SaySomething(const std::string& name, const BT::NodeConfig& config)
      : BT::SyncActionNode(name, config) {}
    
    // It is mandatory to define this STATIC method.
    static BT::PortsList providedPorts() {
      // This action has a single input port called "message"
      return { BT::InputPort<std::string>("message") };
    }

    // Override the virtual function tick()
    BT::NodeStatus tick() override {
      auto msg = getInput<std::string>("message");

      // Check if expected is valid. If not, throw its error
      if (!msg) {
        throw BT::RuntimeError("Missing required input [message]: ", msg.error());
      }

      // use the method value() to extract the valid message.
      std::cout << "SaySomething: " << msg.value() << std::endl;
      return BT::NodeStatus::SUCCESS;
    }
};

class ThinkWhatToSay : public BT::SyncActionNode {
  public:
    ThinkWhatToSay(const std::string& name, const BT::NodeConfig& config)
      : BT::SyncActionNode(name, config) {}

    static BT::PortsList providedPorts() {
      return { BT::OutputPort<std::string>("text") };
    }

    // This Action writes a value into the port "text"
    BT::NodeStatus tick() override {
      // the output may change at each tick(). Here we keep it simple.
      setOutput("text", "Hello World!");
      return BT::NodeStatus::SUCCESS;
    }
};


// ===== Tutorial 03 : Ports with generic types =====

// We want to use this custom type
struct Position2D {
  double x;
  double y;
};

// Template specialization to converts a string to Position2D.
namespace BT {
  template <> inline Position2D convertFromString(BT::StringView str) {
    // We expect real numbers separated by semicolons
    auto parts = BT::splitString(str, ';');
    if (parts.size() != 2) {
      throw BT::RuntimeError("Invalid Position2D format:");
    } else {
      Position2D output;
      output.x = BT::convertFromString<double>(parts[0]);
      output.y = BT::convertFromString<double>(parts[1]);
      return output;
    }
  }
}

class CalculateGoal : public BT::SyncActionNode {
  public:
    CalculateGoal(const std::string& name, const BT::NodeConfig& config)
      : BT::SyncActionNode(name, config) {}

    static BT::PortsList providedPorts() {
      return { BT::OutputPort<Position2D>("goal") };
    }

    BT::NodeStatus tick() override {
      Position2D mygoal = {1.1, 2.3};
      setOutput<Position2D>("goal", mygoal);
      return BT::NodeStatus::SUCCESS;
    }
};

class PrintTarget : public BT::SyncActionNode {
  public:
  PrintTarget(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    // Optionally, a port can have a human readable description
    const char* description = "Imprime la posición objetivo";
    return { BT::InputPort<Position2D>("target", description) };
  }

  BT::NodeStatus tick() override {
    auto res = getInput<Position2D>("target");

    if (!res) {
      throw BT::RuntimeError("Missing required input [target]: ", res.error());
    }

    Position2D target = res.value();
    printf("Posición objetivo: [ %.1f, %.1f ]\n", target.x, target.y);
    return BT::NodeStatus::SUCCESS;
  }
};


// ===== Tutorial 04 : Reactive and Asynchronous behaviors =====

struct Pose2D {
  double x, y, theta;
};

namespace BT {
  template <> inline Pose2D convertFromString(BT::StringView str) {
    auto parts = BT::splitString(str, ';');
    if (parts.size() != 3) {
      throw BT::RuntimeError("Formato Pose2D inválido. Se esperaba 'x;y;theta'");
    } else {
      Pose2D output;
      output.x = BT::convertFromString<double>(parts[0]);
      output.y = BT::convertFromString<double>(parts[1]);
      output.theta = BT::convertFromString<double>(parts[2]);
      return output;
    }
  }
}

class MoveBaseAction : public BT::StatefulActionNode {
  public:
    MoveBaseAction(const std::string& name, const BT::NodeConfig& config)
      : BT::StatefulActionNode(name, config) {}

    static BT::PortsList providedPorts() {
      return { BT::InputPort<Pose2D>("goal") };
    }

    BT::NodeStatus onStart() override;

    BT::NodeStatus onRunning() override;

    void onHalted() override;

  private:
    Pose2D _goal;
    std::chrono::steady_clock::time_point _completion_time;
};



#endif // DUMMY_NODES_H