#ifndef TACTICS_ANIMATIONS_MANAGER_H
#define TACTICS_ANIMATIONS_MANAGER_H

class AnimatedObject;

//================================================================================
// ** Animations
//================================================================================
// Singleton container for all animated objects which self-register and
// de-register on destruction. Implements a local single linked list for optimal
// self-removal and container iteration
//================================================================================
class Animations
{
// Methods
private:
    Animations();
    Animations(const Animations& copy);

public:
    ~Animations();

    static Animations&    instance();
    void                        add(AnimatedObject* obj);
    void                        freeze();
    void                        unfreeze();
    void                        setFrozen(bool frozen);
    bool                        frozen() const;
    void                        update(float elapsed);

// Node Sub-class
    class Node
    {
    // Node Methods
    public:
        Node(AnimatedObject* obj, Animations* container, Node* prev = 0, Node* next = 0);
        ~Node();

        Node*                   previous();
        void                    setPrevious(Node* prev);
        Node*                   next();
        void                    setNext(Node* next);
        AnimatedObject*         getObject();
        const AnimatedObject*   getObject() const;
        
    // Methods
    private:
        AnimatedObject*         object_;
        Node*                   prev_;
        Node*                   next_;
        Animations*             container_;
    };

// Members
    Node*                       front_;
    bool                        frozen_;
};

#endif