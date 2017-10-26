  __    ::
 (_     ::
 (__ \/ ::
::::::::::

Ev simulates evolution. It is quite simple, perhaps disappointingly so.

There are four tile types:

Name     Icon    Foreground    Background    Description
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Empty    .       Blue          Black         An empty tile.

Rock     #       Black         Blue          Block the paths of organisms, immovable and eternal.

Food     ;       Yellow        Green         Provides nutrition for organisms. It spawns randomly,
                                             waiting to be eaten.

Organism J       Cyan          Red           Hopefully the most interesting tile type. They consume
                                             food and reproduce. Offspring have a chance of
                                             mutating, meaning that organisms can evolve over time.


One can interact with the simulation though a set of options:

Name                   Initial    Argument    Default    Runtime?    Description
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Width                  W          uint        200        No          The world's width.

Height                 H          uint        50         No          The world's height.

Organism spawn chance  o          uint 0-255  1          No          The chance out of 255 that a
                                                                     tile will be an organism during
                                                                     world initialization.

Food spawn chance      f          uint 0-255  10         No          The chance out of 255 that a
                                                                     tile will be food during world
                                                                     initialization.

Rock spawn chance      r          uint 0-255  2          No          The chance out of 255 that a
                                                                     tile will be a rock during
                                                                     world initialization.

Brain layers           L          uint,...    16,8       No          The layers of neural networks.

Food nutrition         n          uint        90         No          The points given by food tiles.

Organism fullness      F          uint        1000       No          The food points of new animals. 

Max fullness threshold t          uint        500        No          The maximum of the random
                                                                     fullness threshold generated
                                                                     for new creatures which will
                                                                     alert them if their food points
                                                                     rise above a set point.

Initial mutation       m          float       1.0        No          The amount of variation in new
                                                                     organisms.

Incremental mutation   M          float       0.01       No          The amount of mutation between
                                                                     generations.

Mutation chance        c          uint 0-255  10         No          The chance of mutating babies.

Food per cycle         N          uint        1          Yes         Food tiles added to the world
                                                                     per cycle.

Minimum population     p          uint        30         Yes         The minimum population below
                                                                     which new creatures will appear
                                                                     to compensate.

Organism lifetime      l          uint        2000       No          The number of cycles after
                                                                     which organisms always die.

Cycles per frame       i          uint        100        Yes         The number of cycles per drawn
                                                                     frame.

Seconds per frame      S          uint        0          Yes         The number of seconds per
                                                                     frame.

Nanoseconds per frame  s          uint        80,000,000 Yes         The number of nanoseconds per
                                                                     frame.

Help                   h or ?     None        N/A        No          Displays info a bit like this.

"uint" means a non-negative integer. I didn't want to write "unsigned integer" every time.

Runtime commands can be executed while the simulation is running. First press ENTER to pause the
simulation, then type in the initial followed by the argument with no whitespace inbetween. After
entering some commands, press ENTER again to start the simulation going.


TODO:
 * Add a saving and loading system for creatures and maybe worlds
 * Make it possible to show only a section of a larger world at a time
 * Keep the world from freezing when there is too much food
 * Reorganize the ProgConfig struct
