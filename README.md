# fungus

This program is designed to study the behavior of simple [cellular automata](https://en.wikipedia.org/wiki/Cellular_automaton).

A special case of such a automaton is the [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

The rules of this game are implemented by default in this program.

The only difference from the classic version is the field geometry.

In this program, it is closed, like a sphere.

The application architecture is designed to track the state of each field cell and collect statistics on field states.

The development plans include creating a "life" rule constructor.

It is for these reasons that calculations do not occur as quickly as we would like.

In any case, I'm not a programmer, it's just a hobby for me.

If you can suggest ways to speed up calculations within this architecture, it would be great.
