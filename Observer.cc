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

// The Subject then sends notifications by walking the list and calling methods
class Subject
{
protected:
  void notify(const Entity& entity, Event event)
  {
    for (int i = 0; i < numObservers_; i++)
    {
      observers_[i]->onNotify(entity, event);
    }
  }
  // Other stuff...
}


// Adding and removing observers without any dynamic allocation
// Using 'Linked observers'

// Can thread the reference to the subject through
// the observers themselves

// Put pointer to head of list of observers in Subject
class Subject 
{
  Subject()
  : head_(NULL)
  {

  private:
    Observer* head_;
  }
}

// Extend Observer with a pointer to the next observer in the list:
class Observer
{
  friend class Subject;
  // A friend class can access private and protected members of other class in which it is declared as friend.
public:
  Observer()
  : next_(NULL)
  {}

private:
  Observer* next_;
}

// Registering a new observable means inserting it at front
void Subject::addObserver(Observer* observer)
{
  observer->next_ = head_;
  head_ = observer;
}