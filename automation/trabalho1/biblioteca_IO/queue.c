#include <stdio.h>

#define MAX_QUEUE_SIZE 100

enum color{
    azul = 1,
    verde = 4
}cor;

int queue[MAX_QUEUE_SIZE];
int front = -1, rear = -1;

void enqueue(int value) {
    if (rear == MAX_QUEUE_SIZE - 1) {
        printf("Queue is full.\n");
    } else {
        if (front == -1) {
            front = 0;
        }
        rear++;
        queue[rear] = value;
        printf("Inserted %d into the queue.\n", value);
    }
}

void dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue is empty.\n");
    } else {
        printf("Removed %d from the queue.\n", queue[front]);
        front++;
    }
}

void display() {
    if (front == -1) {
        printf("Queue is empty.\n");
    } else {
        printf("Elements of the queue are: ");
        for (int i = front; i <= rear; i++) {
            printf("%d ", queue[i]);
        }
        printf("\n");
    }
}

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    enqueue(40);
    display();
    dequeue();
    display();
    return 0;
}

/*In this implementation, we have defined an array called queue with a maximum size of MAX_QUEUE_SIZE. We have also defined two variables front and rear to keep track of the position of the front and rear elements of the queue.

The enqueue function inserts a new element into the queue. It checks if the queue is full before inserting the new element. If the queue is not full, it increments the rear variable and adds the new element to the queue.

The dequeue function removes the front element from the queue. It checks if the queue is empty before removing the front element. If the queue is not empty, it removes the front element and increments the front variable.

The display function prints all the elements of the queue.

In the main function, we have added some elements to the queue using the enqueue function, displayed all the elements using the display function, and removed an element from the front of the queue using the dequeue function.

 */



