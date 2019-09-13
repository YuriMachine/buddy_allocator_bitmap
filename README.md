# progettoSO

# What:
This project consists in writing a Buddy Allocator which make use of a bitmap to store the binary tree needed by the allocator.
Storing the binary tree in a bitmap takes less memory and can improve performance thanks to the better cache management.

# How:
My implementation is based on the Buddy Allocator exercise presented in class by our instructors.<br/>
That Buddy Allocator is based on free-lists managed by a SLAB allocator, so the first thing i did was to remove all the reference to the lists so that i could abstract the logic behind Buddy Allocator.<br/>
I chose to implement my own bitmap because of some issues i run into using the one already present in the exercise.<br/>
To port the algorithms to the new data structure I've extensively used binary tree properties to keep the code as simple as possible.<br/>
The "core" of the algorithm was to set parents and childs of the node used to memorize the buddy, the parent algorithm is pretty straightforward, while for the child one i chose to use recursion because of how clean the code actually is, but it can be implemented using a stack too. <br/>

# How to run:
Simply use "make" to compile and run the "buddy_allocator_test" binary.
