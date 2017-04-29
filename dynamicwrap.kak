# The special unicode character is \uE001.
# `d` and `c` change behavior when next char is a \n.
# When next char is \n, deletes char under cursor and
# selects char to left.
# Other wise, deletes char under cursor and selects char to right.
hook global WinDisplay .*\.tex %{
    # I am trying to remove the jk for <esc> becuase it plays poorly with this.
    # It isn't working.
    remove-hooks window bch_ctrl
    
    add-highlighter show_whitespaces
    hook window InsertChar [^\s] %{
         #try %{
         #   exec -no-hooks -draft hH <a-k>jk<ret> d
         #   exec <esc>
         #} catch %{
            exec -no-hooks 
            exec -no-hooks -draft -itersel <a-a>p|kak_dynamicwrap<ret>
            exec -no-hooks <esc>
            exec -no-hooks \%s<ret>
            #exec -no-hooks  L|tee<space>-a<space>~/before.txt<ret>h
            eval -no-hooks -draft -itersel %{
                try %{
                    exec -draft l<a-K>\n<ret>
                    %sh{ echo "_____try" >> ~/after.txt }
                } catch %{
                    exec "li <esc>h"
                    %sh{ echo "_____catch" >> ~/after.txt }
                }
            }
            exec -no-hooks d
            exec -no-hooks L|tee<space>-a<space>~/after.txt<ret>h
           # %sh{ echo "_____" >> ~/after.txt }
            exec -no-hooks i
            #spaceIfNeeded
        #}
    }
}
def spaceIfNeeded %{
    try %{
        exec -no-hooks -iterself l<a-k>\n<ret>i___<esc>h
    }
}
