hook global WinDisplay .*\.tex %{
    remove-hooks window ctrl
    add-highlighter show_whitespaces
    hook window InsertChar [^\s] %{
        try %{
            exec -no-hooks -draft hH <a-k>jk<ret> d
            exec <esc>
        } catch %{
            exec -no-hooks 
            exec -no-hooks -draft -itersel <a-a>p|kak_dynamicwrap<ret>
            exec -no-hooks <esc>
            exec -no-hooks \%s<ret>
            exec -no-hooks c
        }
    }
}
