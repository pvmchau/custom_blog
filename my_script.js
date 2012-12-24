$(document).ready(function(){
  var tabContainers = $('div#custom_manager_blog_content .blog_blogger_content > div');
  $('#custom_invite_manage_blog ul.blog_dashboard a').click(function () {
        tabContainers.hide().filter(this.hash).show();
        $('#custom_invite_manage_blog ul.blog_dashboard a').removeClass('active');
 
        
        $(this).addClass('active');

        return false;
        }).filter(':first').click();
})