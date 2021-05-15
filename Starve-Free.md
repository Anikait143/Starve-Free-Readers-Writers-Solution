It is also called as **Third Readers-Writers** problem. It requires that no thread shall be allowed to starve; that is, 
the operation of obtaining a lock on the shared data will always terminate in a bounded amount of time.

**Problem**:- A Writer thread does not have a chance to execute while any number of 
Readers continuously entering and leaving the working area.

To avoid this problem the following commonly known solution is proposed.

```
//Initialization

in = Semaphore(1)
mx = Semaphore(1)
wrt = Semaphore(1)
ctr = Integer(0)

//Reader

- Wait in
- Wait mx
- if (++ctr)==1, then Wait wrt
- Signal mx
- Signal in
[Critical section]
- Wait mx
- if (--ctr)==0, then Signal wrt
- Signal mx

//Writer

- Wait in
- Wait wrt
- [Critical section]
- Signal wrt
- Signal in
```

This solution is simple and fast enough. However the penalty in comparison to the
common one is that the Reader must lock two mutexes to enter the working area. If
the working area is fast and assuming that mutex locking is a heavy system call, there
would be a benefit of having an algorithm which allows locking one mutex on
entering the working area and one on exiting.

The following presented solution removes the simultaneous use of two mutexes for a
Reader.

```
//Initialization

in = Semaphore(1)
out = Semaphore(1)
wrt = Semaphore(0)
ctrin = Integer(0)
ctrout = Integer(0)
wait = Boolean(0)

//Reader

- Wait in
- ctrin++
- Signal in
[Critical section]
- Wait out
- ctrout++
- if (wait==1 && ctrin==ctrout)
then Signal wrt
- Signal out

//Writer

- Wait in
- Wait out
- if (ctrin==ctrout)
then Signal out
else
- wait=1
- Signal out
- Wait wrt
- wait=0
[Critical section]
- Signal in
```

The main idea here is that a Writer indicates to Readers its necessity to access the
working area. At the same time no new Readers can start working. Every Reader
leaving the working area checks if there is a Writer waiting and the last leaving
Reader signals Writer that it is safe to proceed now. Upon completing access to the
working area Writer signals waiting Readers that it finished allowing them to access
the working area again.
