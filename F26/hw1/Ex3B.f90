program ex3b
    use, intrinsic :: iso_c_binding, only: c_int
    implicit none
    interface
        subroutine z_order(arr, N, rstart, cstart, sz, val) bind(C, name="z_order")
            import :: c_int
            integer(c_int), intent(inout) :: arr(*)
            integer(c_int), value :: N, rstart, cstart, sz, val
        end subroutine z_order
    end interface
    integer :: N, bound, length, i
    integer, allocatable :: arr(:)
    character(20) :: buf
    character(20) :: fmt

    call get_command_argument(1, buf)
    read(buf, *) N

    if (N/=2 .and. N/=4 .and. N/=8 .and. N/=16) then
        print '(A, I0)', "ERROR: N = ", N
        print '(A)', "Must be one of [2, 4, 8, 16]"
        stop 1
    end if

    bound = N*N
    allocate(arr(bound))
    call z_order(arr, N, 0, 0, N, 1)
    write(buf, '(I0)') bound
    length = len_trim(buf)
    write(fmt, '(A, I0, A)') '(*(I', length, ',:,1X))'
    write(*, '(A)', advance='no') 'A=['
    do i = 0, N-1
        if (i > 0) write(*, '(A)', advance='no') '   '
        write(*, fmt, advance='no') arr(i+1 : bound : N)
        if (i < N-1) write(*, '(A)') ''
    end do
    write(*, '(A)') ']'

end program ex3b
