// Code that disentangles events from objects that listen to events and react

// Part 1: The observer
class Observer
{
  public:
      virtual ~Observer() {}
      virtual void onNotify(const Entity& entity, Event event) = 0;
};

// Concrete class that implements this becomes an observer.
class Achievements : public Observer
{
public:
  virtual void onNotify(const Entity& entity, Event event)
  {
    switch (event)
    {
      case EVENT_ENTITY_FULL:
        if (entity.isHero() && heroIsOnBridge_)
        {
          unlock(ACHIEVEMENT_FELL_OFF_BRIDGE);
        }
        break;

        // Handle other events, and update heroIsOnBridge_...
    }
  }

private:
  void unlock(Achievement achievement)
  {
    // Unlock if not already unlocked...
  }

  bool heroIsOnBridge_;
}

// Part 2: The Subject
// (1) Holds the list of observers
// (2) exposts a public API for modifying that list
class Subject
{
private:
  Observer* observers_[MAX_OBSERVERS];
  int numObservers_;

public:
  void addObserver(Observer* observer)
  {
    // Add to array
  }

  void removeObserver(Observer* observer)
  {
    // Remove from array...
  }

  // Other stuff...
}
