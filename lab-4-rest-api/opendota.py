import json
import sys
import urllib.error
import urllib.request

API_BASE = "https://api.opendota.com/api"
USER_AGENT = "Mozilla/5.0 (compatible; simple-opendota-cli/1.0)"
LOG_FILE = "opendota.log"


class Logger:
    def __init__(self, path):
        self.file = open(path, "a", encoding="utf-8")

    def print(self, *args, sep=" ", end="\n", file=sys.stdout):
        text = sep.join(str(arg) for arg in args)
        print(text, end=end, file=file)
        self.file.write(text + end)
        self.file.flush()

    def close(self):
        self.file.close()


def fetch_json(path):
    url = f"{API_BASE}/{path.lstrip('/')}"
    request = urllib.request.Request(url, headers={"User-Agent": USER_AGENT})
    with urllib.request.urlopen(request) as response:
        return json.load(response)


def prompt(text):
    try:
        return input(text).strip()
    except EOFError:
        return ""


def display_matches(matches, logger):
    logger.print()
    logger.print("Recent matches")
    for index, match in enumerate(matches):
        logger.print(
            f"[{index}] "
            f"match_id={match.get('match_id')} "
            f"hero_id={match.get('hero_id')} "
            f"kills={match.get('kills')} "
            f"deaths={match.get('deaths')} "
            f"assists={match.get('assists')} "
            f"win={match.get('radiant_win')}"
        )


def display_match_details(match, logger):
    logger.print()
    logger.print("Match details")
    fields = [
        "match_id",
        "duration",
        "radiant_win",
        "radiant_score",
        "dire_score",
        "game_mode",
        "region",
        "start_time",
    ]
    for field in fields:
        logger.print(f"{field}: {match.get(field)}")

    players = match.get("players") or []
    if players:
        logger.print()
        logger.print("Players")
        for player in players[:10]:
            logger.print(
                f"slot={player.get('player_slot')} "
                f"hero_id={player.get('hero_id')} "
                f"kills={player.get('kills')} "
                f"deaths={player.get('deaths')} "
                f"assists={player.get('assists')}"
            )


def main():
    logger = Logger(LOG_FILE)
    logger.print("OpenDota CLI")
    logger.print("Look up a player's recent matches and inspect one match.")
    logger.print(f"Logging output to {LOG_FILE}")

    try:
        account_id = prompt("Account ID: ")
        if not account_id:
            logger.print("No account ID provided.", file=sys.stderr)
            return 1

        try:
            matches = fetch_json(f"players/{account_id}/recentMatches")
        except urllib.error.HTTPError as error:
            logger.print(
                f"Request failed: {error.code} {error.reason}", file=sys.stderr
            )
            return 1
        except urllib.error.URLError as error:
            logger.print(f"Network error: {error.reason}", file=sys.stderr)
            return 1

        matches = matches[:5]
        if not matches:
            logger.print("No recent matches found.")
            return 0

        display_matches(matches, logger)

        choice = prompt("\nSelect a match number to inspect: ")
        if not choice:
            logger.print("No match selected.")
            return 0

        try:
            selected = matches[int(choice)]
        except (ValueError, IndexError):
            logger.print("Invalid selection.", file=sys.stderr)
            return 1

        match_id = selected.get("match_id")
        if not match_id:
            logger.print("Selected match has no match_id.", file=sys.stderr)
            return 1

        try:
            match = fetch_json(f"matches/{match_id}")
        except urllib.error.HTTPError as error:
            logger.print(
                f"Request failed: {error.code} {error.reason}", file=sys.stderr
            )
            return 1
        except urllib.error.URLError as error:
            logger.print(f"Network error: {error.reason}", file=sys.stderr)
            return 1

        display_match_details(match, logger)
        return 0
    finally:
        logger.close()


if __name__ == "__main__":
    raise SystemExit(main())
