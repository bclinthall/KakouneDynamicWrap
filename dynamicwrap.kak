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
            eval -no-hooks -draft -itersel %{
                try %{
                    # trigger catch block if char after  is \n
                    exec -draft l<a-K>\n<ret> 
                } catch %{
                    # insert a space after 
                    exec "li <esc>h"
                }
            }
            exec -no-hooks d
            exec -no-hooks L|tee<space>-a<space>~/after.txt<ret>h
            exec -no-hooks i
        #}
    }
}
