hook global WinDisplay .*\.tex %{
    add-highlighter show_whitespaces
    hook window InsertChar [^\n] %{
        dynamicwrap
    }
}
def dynamicwrap %{
    exec -no-hooks <esc><a-a>p|kak_dynamicwrap<ret>s<ret>c
}
