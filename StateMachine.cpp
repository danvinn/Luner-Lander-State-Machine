C++ script for a lunar lander state machine

#include <iostream>
#include <vector>
#include <memory>

// Abstract Observer
class Observer {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~Observer() {}
};

// Abstract State
class State {
public:
    virtual void handle(class LunarLanderContext& context) = 0;
    virtual ~State() {}
};

// Forward declaration for use in LunarLanderContext
class StateFactory;

// Singleton Context
class LunarLanderContext {
    std::unique_ptr<State> currentState;
    static LunarLanderContext* instance;
    std::vector<Observer*> observers;

    // Private constructor for Singleton
    LunarLanderContext();

public:
    static LunarLanderContext* getInstance();
    void setState(State* state);
    void request();
    void notifyObservers(const std::string& message);
    void attachObserver(Observer* observer);
    ~LunarLanderContext();
};

// StateFactory for creating states
class StateFactory {
public:
    static State* createState(const std::string& type);
};

// Concrete States
class DeployedInOrbit : public State {
public:
    void handle(LunarLanderContext& context) override;
};

class Descending : public State {
public:
    void handle(LunarLanderContext& context) override;
};

class EngineCutOff : public State {
public:
    void handle(LunarLanderContext& context) override;
};

class Landed : public State {
public:
    void handle(LunarLanderContext& context) override;
};

// Implementations of StateFactory
State* StateFactory::createState(const std::string& type) {
    if (type == "DeployedInOrbit") return new DeployedInOrbit();
    if (type == "Descending") return new Descending();
    if (type == "EngineCutOff") return new EngineCutOff();
    if (type == "Landed") return new Landed();
    return nullptr;
}

// Implementations of LunarLanderContext methods
LunarLanderContext* LunarLanderContext::instance = nullptr;

LunarLanderContext::LunarLanderContext() : currentState(StateFactory::createState("DeployedInOrbit")) {}

LunarLanderContext* LunarLanderContext::getInstance() {
    if (instance == nullptr) {
        instance = new LunarLanderContext();
    }
    return instance;
}

void LunarLanderContext::setState(State* state) {
    currentState.reset(state);
    std::cout << "State transitioned.\n";
}

void LunarLanderContext::request() {
    currentState->handle(*this);
}

void LunarLanderContext::notifyObservers(const std::string& message) {
    for (auto observer : observers) {
        observer->update(message);
    }
}

void LunarLanderContext::attachObserver(Observer* observer) {
    observers.push_back(observer);
}

LunarLanderContext::~LunarLanderContext() {}

// Concrete Observer
class DiagnosticTool : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << "Diagnostic Tool: " << message << std::endl;
    }
};

// State behaviors with error handling and notifications
void DeployedInOrbit::handle(LunarLanderContext& context) {
    try {
        std::cout << "Lunar Lander is deployed in orbit. Preparing for descent.\n";
        context.notifyObservers("Deployed in Orbit: Preparing for descent");
        context.setState(StateFactory::createState("Descending"));
    } catch (...) {
        context.notifyObservers("Error while deploying in orbit.");
    }
}

void Descending::handle(LunarLanderContext& context) {
    try {
        std::cout << "Lunar Lander is descending. Engines are operational.\n";
        context.notifyObservers("Descending: Engines are operational");
        context.setState(StateFactory::createState("EngineCutOff"));
    } catch (...) {
        context.notifyObservers("Error during descent.");
    }
}

void EngineCutOff::handle(LunarLanderContext& context) {
    try {
        std::cout << "Engines cut off at 30 meters above the surface. Final descent phase.\n";
        context.notifyObservers("Engine Cut Off: At 30 meters");
        context.setState(StateFactory::createState("Landed"));
    } catch (...) {
        context.notifyObservers("Error during engine cut-off.");
    }
}

void Landed::handle(LunarLanderContext& context) {
    try {
        std::cout << "Lunar Lander has landed on the surface. Mission successful.\n";
        context.notifyObservers("Landed: Mission successful");
    } catch (...) {
        context.notifyObservers("Error during landing phase.");
    }
}

// Main function to demonstrate state transitions with observer
int main() {
    LunarLanderContext* lander = LunarLanderContext::getInstance();
    DiagnosticTool diagnosticTool;
    lander->attachObserver(&diagnosticTool);

    try {
        lander->request(); // Transition from DeployedInOrbit to Descending
        lander->request(); // Transition from Descending to EngineCutOff
        lander->request(); // Transition from EngineCutOff to Landed
    } catch (const std::exception& e) {
        std::cout << "An exception occurred: " << e.what() << std::endl;
        lander->notifyObservers("An exception occurred in the state machine.");
    } catch (...) {
        std::cout << "An unknown error occurred." << std::endl;
        lander->notifyObservers("An unknown error occurred in the state machine.");
    }

    delete lander; // Cleanup Singleton instance
    return 0;
}




//Key Points of the Complete Script
//State Pattern: The script utilizes the state pattern to manage different states of the lunar lander, such as being in orbit, descending, and landing.

//Singleton Pattern: The LunarLanderContext is implemented as a singleton to ensure there is only one control system managing the state transitions.

//Observer Pattern: A diagnostic tool is implemented as an observer which gets notifications about the status and any issues in each state of the lunar lander. This is crucial for monitoring and diagnostics, especially in real-time systems where immediate response may be necessary.

//Factory Pattern: A StateFactory class is used to create instances of different states, making it easy to add new states or modify existing ones without changing the client code.

//Error Handling: Basic error handling is incorporated into each state transition to catch and notify any operational anomalies during the mission's phases. This ensures that the system can gracefully handle unexpected situations.

//Debugging and Notifications: Debug print statements and notifications provide insights into the lander’s operations at each stage, which are crucial for debugging and for operational transparency in real missions.

//This full script provides a structured approach to managing a lunar lander's operations using design patterns to enhance maintainability, scalability, and robustness of the software architecture. The script can be further expanded or modified to integrate more complex behaviors and interactions as needed for more detailed mission requirements.
