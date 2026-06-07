# OpenDota CLI

## Application

This project is a small Python command line program that reads public data from the OpenDota API. The program is meant for simple exploration, not full analysis. A user enters a player account ID, sees a short list of recent matches, and then selects one match to inspect.

The script shows only a few basic fields so the output stays easy to read. For recent matches, it shows match ID, hero ID, kills, deaths, assists, and whether Radiant won. For a selected match, it shows top-level match fields like duration, score, game mode, region, and start time. It also prints a short player summary for the match.

## Design

The design is intentionally small. The program uses Python's built-in `urllib` and `json` modules, so it does not need extra packages. A helper function sends requests to the OpenDota API and returns parsed JSON data. The request includes a `User-Agent` header because the API returned a 403 error when Python used its default request headers.

The program is interactive. It prompts the user for an account ID, fetches the five most recent matches, and then asks which match to inspect. This keeps the workflow simple and avoids too many command line arguments.

The script also uses a small logger wrapper. The wrapper prints output to the terminal and writes the same output to `opendota.log`. This makes it easy to keep a record of each session without adding a complex logging system.

## Usage

Run the program from the project directory:

```bash
python3 opendota.py
```

Steps:

1. Enter an OpenDota account ID.
2. Review the list of recent matches.
3. Enter the match number you want to inspect.
4. Read the match details on screen or in `opendota.log`.

If the account ID is invalid or the network request fails, the program prints a short error message. This project is useful for quick match lookup and simple inspection of a few important fields.
