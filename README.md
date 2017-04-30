# KakouneDynamicWrap

Say you're writing in plaintext or tex and you want your lines automatically 
broken at 80 characters.  As you type, this plugin takes the paragraph you are working 
on, and breakes each of its lines at sensible places short of 80 character. It is intended 
as an alternative to softwrap.

## Installation

Clone the project into your kakoune `autoload` directory.  The change directories into
it and run `make`.  Next you'll need to get the `kak_dynamicwrap` executable onto your path.
Then dynamic wrap should work.

If you haven't yet setup `$XDG_CONFIG_HOME/kak/autoload`, be forewarned that you
might need to put simlinks to `.../kak/rc/*` in your autoload folder. https://github.com/mawww/kakoune/issues/1


