# snake_qlearning

`snake_qlearning` is a project that use the [Q-Learning](https://en.wikipedia.org/wiki/Q-learning) algorithm to ensure that the snake plays the game most efficiently. Q-Learning is one of the reinforcement learning algorithms and aims to help a snake learn the most appropriate movements while collecting rewards (points) and avoiding obstacles (walls or its own tail) on the playing field.

## Setup

1. Clone or download project files:

```
git clone https://github.com/username/snake_qlearning.git
cd snake_qlearning
```

2. Use the Makefile to compile and run the project:

```
make
```

## Usage

### Arguments

| Argument               | Description                                                                                                   |
|------------------------|---------------------------------------------------------------------------------------------------------------|
| **learning_rate**      | The rate at which the snake learns by updating its Q-values (suggested range: 0.1 - 0.5).                    |
| **discount_factor**    | The factor that discounts future rewards, balancing immediate vs. long-term rewards (e.g., 0.9).             |
| **max_exploration_rate** | The initial probability of exploring (choosing random actions) rather than exploiting learned knowledge.     |
| **min_exploration_rate** | The minimum exploration rate to prevent the snake from getting stuck with limited actions.                    |
| **exploration_decay**  | The rate at which exploration decreases over time, helping the snake to settle on learned strategies.        |
| **width**              | The width of the game board.                                                                                 |
| **height**             | The height of the game board.                                                                                |
| **episodes**           | The number of episodes (games) for training the snake to optimize its performance.                           |
| **table_size**         | The size of the Q-table to store state-action pairs for learned behaviors.                                   |


### Run
```
./snake_qlearning \
    learning_rate=.1 \
    discount_factor=.9 \
    max_exploration_rate=1 \
    min_exploration_rate=.01 \
    exploration_decay=.998 \
    width=10 \
    height=10 \
    episodes=1000000 \
    table_size=2000000
```

### Output

After training, the program will generate a model file containing the learned Q-values. This file allows the snake to use the trained model for efficient gameplay without retraining. The model can also be reloaded or updated with further training to improve its performance.
