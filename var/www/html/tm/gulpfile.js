
var gulp = require('gulp')


var uglify = require('gulp-uglify')
var minifyCSS = require('gulp-minify-css')

gulp.task('script', function() {
    gulp.src('asset/dist/js/*.js')
        .pipe(uglify())
        .pipe(gulp.dest('asset/js'))
})

gulp.task('css', function () {
    gulp.src('asset/dist/css/*.css')
        .pipe(minifyCSS())
        .pipe(gulp.dest('asset/css'))
})

gulp.task('auto', function () {
    gulp.watch('js/*.js', ['script'])
    gulp.watch('css/*.css', ['css'])
})

gulp.task('default', ['script', 'css' , 'auto'])
