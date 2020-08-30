# The Qt Library Management System

![scrot](https://github.com/arvl130/qlibmgsys/blob/dev/scrot.png)

This program was written back in 2019 for a school project that we needed to finish for the semester.

The code was written in haste, which explains the spaghettified code, and not much has changed since
the semester has finished, and the project was abandoned.

I am hosting this project here now, so that I can continue on the work that was previously done,
and perhaps improve the code, such that more people would find it more useful.

I am distributing it free of charge in the hopes that it will be useful, but **_WITHOUT WARRANTY_**.
Without even the implied warranty of merchantability or fitness for a particular purpose.

SO feel free to use this for your own school projects, but don't blame me if things don't always
work the way you want. :P

That said, anyone having problems is free to post issues in the issue tracker, as well as submit
code, as long as it doesn't diverge too much from the current codebase, and comes with proper
explanations.

Needless to say, this repo was made for development so **EXPECT BUGS**, and be sure to report them
when you find them.

## Current Goals
* Refactor UI to be more usable
* Refactor all CRUD functions such that we don't have to embed all queries in ViewWindow
* Implement a setup wizard for setting the admin's password on first launch
* more things I can't remember right now ...

## Stretch Goals
* Allow custom database backends (MariaDB/MySQL, PostgreSQL, MongoDB)
* Allow fetching book metadata from remote sites (Amazon, Google?)
* Allow importing/exporting database to/from a more human-friendly format (JSON?)
* Flexible book attributes
* Password recovery options (?)
* Implement a borrower tracking system

#### Copyright (C) 2019-2020 Angelo Geulin, Gabriel Andrei Ecleo
