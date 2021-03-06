kdb-find(1) -- Find keys in the key database
================================

## SYNOPSIS

`kdb find <regex>`

Where `regex` is a regular expression which contains the key to find.

## DESCRIPTION

This command will list the name of all keys that contain `regex`.

## OPTIONS

- `-H`, `--help`:
  Show the man page.
- `-V`, `--version`:
  Print version info.
- `-p`, `--profile <profile>`:
  Use a different kdb profile.
- `-C`, `--color <when>`:
  Print never/auto(default)/always colored output.
- `-v`, `--verbose`:
  Explain what is happening.
- `-0`, `--null`:
  Use binary 0 termination.

## EXAMPLES

```sh
# Backup-and-Restore: /tests/find

# We use the `dump` plugin, since some storage plugins, e.g. INI,
# create intermediate keys, such as `/tests/find/tests/foo`
# for the following test.
sudo kdb mount find.ecf /tests/find dump

# Create the keys we use for the examples
kdb set /tests/find/tests val1
kdb set /tests/find/tests/foo/bar val2
kdb set /tests/find/tests/fizz/buzz fizzbuzz
kdb set /tests/find/tostfizz val3
kdb set /tests/find/tust/level lvl

# list all keys containing /tests/find/t[eo]
kdb find '/tests/find/t[eo]'
#> user/tests/find/tests
#> user/tests/find/tests/fizz/buzz
#> user/tests/find/tests/foo/bar
#> user/tests/find/tostfizz

# list all keys containing fizz
kdb find 'fizz'
#> user/tests/find/tests/fizz/buzz
#> user/tests/find/tostfizz

sudo kdb umount /tests/find
```

## SEE ALSO

- If the user would also like to see the values of the keys below `path` then you should
consider the [kdb-export(1)](kdb-export.md) command.
- [elektra-key-names(7)](elektra-key-names.md) for an explanation of key names.
