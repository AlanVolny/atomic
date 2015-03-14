Atomic: an experiment / simulation with 'atoms' that bond and repel & attract.

========

Dependencies:
[make](http://www.gnu.org/software/make/), [gcc](https://gcc.gnu.org/). 

cd into the directory, type 'make', run atomic.

=======

To change the number or type of atoms, the friction coefficient, or the speed/granularity of the simulation, go to **atomic.cpp** and change the relevant lines of code.
To change the basic characteristics of elements, go to **atom.cpp** and change the values for the atoms

Some terminals need to run the simulation slower/at a more granular level. Xterm or similarly speedy terminal is recommended, otherwise you can turn down the granularity in **atomic.cpp**

=========

**General**

Elements have characteristics (minimum_bond_length, maximum_bond_length, ideal_bond_length, bond_strength, compress_resistForce, weight). When an atom is withn maximum_bond_length of another atom, they 'bond', and until they are unbonded, they push/pull on each other to try to reach their common ideal bond length. If they are too close, past their common minimum bond length, they start using their compress_resistForce to push each away, instead. Weight is only used for inertia, to resist changes in velocity.

This was made some while ago in c++, to see whether materials and their properties could be reliably recreated using very simple atom-like particles. 

=========

**screenshots:**

![Metal atoms tend to form stable shapes](http://i.imgur.com/5f7UeCd.png)
![Metal after some period of time, and losing some energy to friction](http://i.imgur.com/lYJFfJY.png)
![A blob of water, right after simulation start. Water attracts from away, and is happy being very close to other atoms](http://i.imgur.com/P3AX2CR.png)
![A blob of water about 1 sec after simulation start on high friction. It turns into an round-ish blob, that never really seems to settle down.](http://i.imgur.com/zKClHkV.png)
![Air repels itself, and wants to be as far away from itself as possible. However, due to how bonds are calculated, it still can be attracted to other elements](http://i.imgur.com/BV5o0Ds.png)
![Element 'x', this one will form 'double atom' bonds, where there are two atoms in the place of one. Slightly increasing decompress force removes this tendency, however](http://i.imgur.com/zNMCWIi.png)
![Element X and metal alloy. Each star is multiple atoms in the same position. Element X still shows it's tendency to double atom bound](http://i.imgur.com/BsAL9nf.png)