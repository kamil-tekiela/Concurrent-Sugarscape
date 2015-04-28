project
=======


A sugarscape implementation by Kamil Tekiela as a 4th year software development project in IT Carlow. 

Sugarscape is a model artificially intelligent agent-based social simulation following some or all rules presented by Joshua M. Epstein & Robert Axtell in their book Growing Artificial Societies.

All Sugarscape models include the agents (inhabitants), the environment (a two-dimensional grid) and the rules governing the interaction of the agents with each other and the environment.

The original model presented by J. Epstein & R. Axtell (considered as the first large scale agent model) is based on a 51x51 cell grid, where every cell can contain different amounts of sugar (or spice). In every step agents look around, find the closest cell filled with sugar, move and metabolize. They can leave pollution, die, reproduce, inherit sources, transfer information, trade or borrow sugar, generate immunity or transmit diseases - depending on the specific scenario and variables defined at the set-up of the model.

Sugar in simulation could be seen as a metaphor for resources in an artificial world through which the examiner can study the effects of social dynamics such as evolution, marital status and inheritance on populations.

Exact simulation of the original rules provided by J. Epstein & R. Axtell in their book can be problematic and it is not always possible to recreate the same results as those presented in Growing Artificial Societies.
