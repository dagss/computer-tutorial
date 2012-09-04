module transpose
  implicit none
contains
  subroutine transpose_naive(input, output, n)
    real(8), dimension(n, n) :: input, output
    integer :: n, i, j

    do j = 1, n
       do i = 1, n
          output(j, i) = input(i, j)
       end do
    end do
  end subroutine transpose_naive

  subroutine transpose_tiled(input, output, n)
    integer, parameter :: t = 32
    real(8), dimension(n, n) :: input, output
    integer :: n, itile, jtile, i, j

    ! Note: This obviously fails if n does not divide tilesize;
    ! fixing this up is left as an exersize to the reader...
    do jtile = 0, n / t - 1
       do itile = 0, n / t - 1
          do j = 1, t
             do i = 1, t
                output(jtile * t + j, itile * t + i) = &
                     & input(itile * t + i, jtile * t + j)
             end do
          end do
       end do
    end do
  end subroutine transpose_tiled

end module transpose


program transpose_runner
  use transpose
  use omp_lib
  implicit none

  integer, parameter :: n = 8192
  real(8) :: t0, dt_naive, dt_tiled
  real(8), dimension(:, :), allocatable :: input, output
  integer :: i, j

  allocate(input(n, n))
  allocate(output(n, n))

  do j = 1, n
     do i = 1, n
        input(j, i) = (j - 1) * n + (i - 1)
     end do
  end do

  write (*,*)
  write (*,*) "First two columns of input:"
  do j = 1, 10
     write (*,"(F10.0,F10.0)") input(j, 1), input(j, 2)
  end do
  write (*,*) "..."
  do j = n - 10, n
     write (*,"(F10.0,F10.0)") input(j, 1), input(j, 2)
  end do

  write (*,*)

! Call transpose_naive
  t0 = omp_get_wtime()
  call transpose_naive(input, output, n)
  dt_naive = omp_get_wtime() - t0
  write (*,'(A,ES14.3)') "transpose_naive running time: ", dt_naive

! Call transpose_tiled
  t0 = omp_get_wtime()
  call transpose_tiled(input, output, n)
  dt_tiled = omp_get_wtime() - t0
  write (*,'(A,ES14.3)') "transpose_tiled running time: ", dt_tiled
  write (*,*) "speedup", dt_naive / dt_tiled

  write (*,*)
  write (*,*) "First two colums of output:"
  do j = 1, 10
     write (*,"(F10.0,F10.0)") output(j, 1), output(j, 2)
  end do
  write (*,*) "..."
  do j = n - 10, n
     write (*,"(F10.0,F10.0)") output(j, 1), output(j, 2)
  end do

  deallocate(input)
  deallocate(output)

end program transpose_runner
