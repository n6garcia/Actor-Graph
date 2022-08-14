# Actor-Graph
## Intro
Coded program for playing an optimal game of "Six Degrees of Kevin Bacon" from 2019 IMDB database. 

## Six Degrees of Kevin Bacon
The game is simple try and find a list of actors from the first to the last where actors can only be chosen if they've played in a movie with the last actor chosen.

## How it works
Program populates graph with IMDB database where links contain information on what movies they worked together with and nodes contain actor names. Program finds shortest path from one node to another and uses that path to concatenate a list of movies and actors that will lead to an optimal path from one actor to another.
