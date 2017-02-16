/**
 * The contents of this file are only a suggestion.
 * Feel free to change anything.
 */

#include "matcher.h"

/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
  // You can use this recommended helper function, or not.
  // Currently using this as a boolean.
  if (*pattern == '\0' || *partial_line == *pattern) {
    return 1;
  }
  return 0;
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */
int rgrep_matches(char *line, char *pattern) {

    //
    // TODO put your code here.
    //
    /* Tokens guide
    if (pattern[i] == '\.'), check if there is a character
    if (pattern[i] == '\+'), check if character is same as last, 
      if so, keep checking; if not, increment both pointers
    if (pattern[i] == '\?'), use a check in the preceding not matching character;
      if "next" == \?, then increment both
    if (pattern[i] == '\\'), treat very next token as normal character
    */

  /* For documentation purposes, let's call each variable the following:
      *line - the character
      *pattern - the token
     The token will be explicitly stored as such. How nice!

     Unless the token is '\', we check both the current and the next token, 
     as '+' and '?' require knowledge of the directly preceeding character.

     For '\', we actually need to check for a next next token, as the next token
     is really part of the '\' token. '+' and '?' must be considered when looking 
     past the next token.

     Valid matches are when the character and token match, or if the token is '.'
     and there is any character in a corresponding position.
  */

  char token = *pattern;
  if (token == '\0') { // end of pattern?
    return 1; // wow that was fast! (jk you probably got here from multiple recursions. yay compact code! good thing we don't have to worry about long lines.)
  } else if (token == '\\') { // if the token is '\' we nullify any special meaning for the NEXT token. ergo this must be first check
    // need to account for \.? cases
    if (*(pattern + 2) == '?') { // This is hard-coded in. If possible, make '?' handler a helper function to prevent bugs caused by changing this code
      if (matches_leading(line,pattern+3) && !matches_leading(line+1,pattern+3)) { // if the character matches NEXT non-'?' token and NEXT character does not
        return rgrep_matches(line, pattern+3); // we recurse with the aformentioned elements
      } else if (*line == *(pattern + 1)) {
        return rgrep_matches(line+1, pattern+3);
      } else {
        return rgrep_matches(line, pattern+3);
      }
    } else if (matches_leading(line, pattern+1)) { // test to see if the next character and token match - THIS MIGHT BE SECTION OF ERROR (lol it was; i was returning matches_leading)
      return rgrep_matches(line+1, pattern+2); // THERE THAT SHOULD FIX IT. JK '.+\+' pattern doesn't work for sure.
    } else { // no go? 
      return 0;
    }
  } else if (*(pattern + 1) == '+') { // if the NEXT token is '+' current token must show up once or more
    // Consider '...+\.' cases
    if ((*line == token) || (token == '.' && *line != '\0')) { // if there's a valid match
      if (*(pattern + 2) == '\\' && matches_leading(line + 1, pattern + 3)) { // if the NEXT non-'+' token is '\', check if the NEXT NEXT token matches the NEXT charater
        return rgrep_matches(line+1, pattern+2);
      } else if (*(pattern + 2) == '.' || matches_leading(line + 1, pattern + 2)) { // if the NEXT character and NEXT non-'+' token match. this is the first check as a precaution for '.+.' patterns. 
        return rgrep_matches(line+1, pattern+2); // recurse with that character and that token. could probably be changed in the future, but is stable enough for now
      } else { // if the NEXT character and NEXT non-'+' token do not match
        return rgrep_matches(line+1, pattern); // recurse with the NEXT character and the current token. this is to check the NEXT NEXT character (if there's a valid match this time)
      }
    } else { // if there is no valid match it's a no go
      return 0; // we return 0
    }
  } else if (*(pattern + 1) == '?') { // if the NEXT token is '?' current character is optional. rules for this are somewhat tricky
    if (matches_leading(line,pattern+2) && !matches_leading(line+1,pattern+2)) { // if the character matches NEXT non-'?' token, and NEXT character does not match NEXT non-'?' token. (IDK WHY IT'S GOTTA BE WEIRD LIKE THIS)
      // Could probably account for more repeating letter edge cases by adding a while loop here.
      return rgrep_matches(line,pattern+2); // we recurse with the aforementioned elements. see above note about recurse (in '+')
    } else if (*line == token) { // if the above case isn't true, we check if it's because character actually matches token
      return rgrep_matches(line+1, pattern+2); // dandy! let's move on
    } else { // then again maybe they don't actually match
      return rgrep_matches(line, pattern+2); // no problem! it was extra credit, anyway
    }
  } else if ((*line == token) || (token == '.' && *line != '\0')) { // otherwise we check for valid matches normally
    return rgrep_matches(line+1, pattern+1); // if we're good, let's move on
  } else { // we've exhausted all possibilities
    return 0; // You say you match the pattern? OBJECTION!
  }
}
