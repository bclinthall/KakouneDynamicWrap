# The special unicode character is \uE001.

# allows 'jk' to function as <esc>
# hook global WinDisplay .* %{
#    hook -group bch_ctrl window InsertChar k %{ try %{
#        exec -no-hooks -draft hH <a-k>jk<ret> d
#        exec <esc>
#    }}
#}
#hook global WinDisplay (.*\.tex|.*\.txt) %{
#    # removes above hook
#    remove-hooks window bch_ctrl 
#    hook -group dynamicwrap window InsertChar [^\s] %{
#        try %{ # checks for 'jk' and does <esc> if found.
#            exec -no-hooks -draft hH<a-k>jk<ret>d
#            exec <esc>
#        } catch %{ # otherwise, do dynamic wrap
#            dynamicwrap
#        }
#    }
#}

hook global WinDisplay (.*\.tex|.*\.txt|.*\.md) %{
    hook -group dynamicwrap window InsertChar [^\s] %{
        dynamicwrap
    }
}


def dynamicwrap %{
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
}
