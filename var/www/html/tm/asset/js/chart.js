$(function(){
    function a(a){
        var t,e=a.getYear();
        e<1900&&(e+=1900);
        var n=a.getMonth()+1;
        n<10&&(n="0"+n);
        var i=a.getDate();
        return i<10&&(i="0"+i),t=e+"-"+n+"-"+i
    }
    function t(a,t){
        $.getJSON(a,function(a){
            var e=[],n=[],i=[];
            $.each(a,function(a,o){
                a%t==0&&(e.push(o.time),n.push(o.tmp),i.push(o.hmt))});
            
            var o=echarts.init(document.getElementById("main"));
            option={
                title:{text:"",subtext:""},
                tooltip:{trigger:"axis"},
                legend:{data:["Temperature","Humidity"]},
                toolbox:{show:!0,feature:{dataZoom:{yAxisIndex:"none"},
                    dataView:{readOnly:!1},
                    magicType:{type:["line","bar"]},
                    restore:{},saveAsImage:{}}},
                // grid: {
                //     left: '10%',   // Adjust this as needed for label spacing
                //     right: '10%',  // Adjust this as needed for label spacing
                //     bottom: '10%',
                //     containLabel: true // Ensures that labels are not cut off
                // },
                xAxis:{type:"category",boundaryGap:!1,data:e},
                yAxis:[{
                    type:"value",
                    name: 'Tmp °C',
                    scale: true,
                    splitNumber: 6,
                    position: 'left'    
                    },{
                    type:"value",
                    name: 'Hmd %',
                    position: 'right'
                    }],
                series:[{
                        name:"Temperature",
                        type:"line",
                        data:n,
                        markPoint:{data:[{type:"max",name:"Max"},{type:"min",name:"Min"}]}
                    },{
                        name:"Humidity",
                        type:"line",
                        yAxisIndex: 1,
                        data:i,
                        markPoint:{data:[{type:"max",name:"Max"},{type:"min",name:"Min"}]}}]}
                    
                    ,o.setOption(option)})
    }

    var e=new Date,n="hour",i="data/"+n+"/"+a(e)+".json";
    t(i,1),$("#singleDateRange").DatePicker({startDate:moment()}),$("#submitit").click(function(){var a=$("#singleDateRange").val();n="min";var e="data/"+n+"/"+a+".json";t(e,1)}),$("#changeit").click(function(){var a=$("#singleDateRange").val();n="hour";var e="data/"+n+"/"+a+".json";t(e,1)})});