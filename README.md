# DD-HHConverter
Hand History converter to 888 Poker HH format for DD Poker simulator (www.ddpoker.com) to import on PokerTracker 4.


- Insert the hand history and summary to convert in original DD HH and summary file respectively named "history.paradise.txt" and "tournaments.csv", copy/paste, or replace, but the files must be called like that.
- The files must be in the same folder as the exe.
- Run the exe, the converted hh and summary will be written in "history.888.txt" and "tournaments.888.txt", ready to import. 
  (will be overwritten every time)
- Use the ini file "HHConverter.ini" to manually put buy-in division factor (to have the possibilty to import on PT4 with small stacks license), fee %, tourney size (X-Max) and currency. Due to DD HH limitations conversion can only be done for one type of tourney format at once and you can't mix different type of tourney, to use conversion for a different format you must update manually tourney format in ini file and relaunch conversion.
  (each DD HH you want to convert to 888 HH must be for one tourney format and ini file updated with this tourney format)

* I noticed a bug we can't deal with in DD HH if you are on seat 10 and in button, DD HH shows seat 1 is in button instead of seat 10 despite it correctly recognizes you are in seat 10 like this:

```
Game #1012740 - Tournament DD Poker Spin&Go - 15/30 No Limit Texas Hold'em - 2022/02/09-08:19:01.3 (CST)
Table "DD Poker Spin&Go 1" Hand #11 -- Seat 1 is the button
Seat  4: Ashton  (665 in chips)
Seat  8: Caroline  (385 in chips)
Seat 10: Mudr0x  (450 in chips)
Mudr0x: Ante (5)
Ashton: Ante (5)
Caroline: Ante (5)
Ashton: Post Small Blind (15)
Caroline: Post Big Blind (30)
Dealing...
```
* As written in C++ time execution is incredible compared to other converters! Less than 1 min for 11k hands:

![Screenshot (49)](https://user-images.githubusercontent.com/78977383/166441822-d081be3a-2c90-4ea3-b9ad-b6833c5bee88.png)


**WARNING! DO NOT USE NICKNAMES WITH WHITESPACES ON DD POKER FOR HH CONVERSION ELSE YOU WILL HAVE BAD CONVERTED FILES WHICH YOU COULDN'T IMPORT ON PT4 (generating only errors).**
