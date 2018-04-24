internal function Boolean initialize_Windows_sockets()
{
    if(!windows_sockets.version)
        return !WSAStartup(0x0202, &windows_sockets);

    return 1;
}


internal procedure deinitialize_Windows_sockets()
{
    if(windows_sockets.version)
        WSACleanup();

    windows_sockets.version = 0;
}