/*
jQWidgets v2.4.1 (2012-Sep-07)
Copyright (c) 2011-2012 jQWidgets.
License: http://jqwidgets.com/license/
*/

(function(a){a.jqx.jqxWidget("myWidget","",{});a.extend(a.jqx._myWidget.prototype,{var1:5,var2:10,foo:function(b){this.var1*=b},bar:function(){alert(this.var1)},createInstance:function(b){}});a.jqx.jqxWidget("jqxScrollBar","",{});a.extend(a.jqx._jqxScrollBar.prototype,{defineInstance:function(){this.height=null;this.width=null;this.vertical=false;this.min=0;this.max=1000;this.value=this.min;this.step=10;this.largestep=50;this.thumbMinSize=10;this.thumbSize=0;this.roundedCorners="all";this.showButtons=true;this.disabled=false;this.touchMode="auto";this.touchModeStyle="auto";this.thumbTouchSize=8;this._triggervaluechanged=true},createInstance:function(c){var b=this;this.host.append("<div id='jqxScrollOuterWrap' style='width:100%; height: 100%; align:left; border: 0px; valign:top; position: relative;'><div id='jqxScrollWrap' style='width:100%; height: 100%; left: 0px; top: 0px; align:left; valign:top; position: absolute;'><div id='jqxScrollBtnUp' style='align:left; valign:top; left: 0px; top: 0px; position: absolute;'/><div id='jqxScrollAreaUp' style='align:left; valign:top; left: 0px; top: 0px; position: absolute;'/><div id='jqxScrollThumb' style='align:left; valign:top; left: 0px; top: 0px; position: absolute;'/><div id='jqxScrollAreaDown' style='align:left; valign:top; left: 0px; top: 0px; position: absolute;'/><div id='jqxScrollBtnDown' style='align:left; valign:top; left: 0px; top: 0px; position: absolute;'/></div></div>");if(this.width!=undefined&&parseInt(this.width)>0){this.host.width(parseInt(this.width))}if(this.height!=undefined&&parseInt(this.height)>0){this.host.height(parseInt(this.height))}this.btnUp=this.host.find("#jqxScrollBtnUp");this.btnDown=this.host.find("#jqxScrollBtnDown");this.btnThumb=this.host.find("#jqxScrollThumb");this.areaUp=this.host.find("#jqxScrollAreaUp");this.arrowUp=a("<div></div>");this.arrowUp.appendTo(this.btnUp);this.arrowDown=a("<div></div>");this.arrowDown.appendTo(this.btnDown);this.areaDown=this.host.find("#jqxScrollAreaDown");this.scrollWrap=this.host.find("#jqxScrollWrap");this.scrollOuterWrap=this.host.find("#jqxScrollOuterWrap");this.btnUp[0].id="jqxScrollBtnUp"+this.element.id;this.btnDown[0].id="jqxScrollBtnDown"+this.element.id;this.btnThumb[0].id="jqxScrollThumb"+this.element.id;this.areaUp[0].id="jqxScrollAreaUp"+this.element.id;this.areaDown[0].id="jqxScrollAreaDown"+this.element.id;this.scrollWrap[0].id="jqxScrollWrap"+this.element.id;this.scrollOuterWrap[0].id="jqxScrollOuterWrap"+this.element.id;if(!this.host.jqxRepeatButton){alert("jqxbuttons.js is not loaded.");return}this.btnUp.jqxRepeatButton({overrideTheme:true});this.btnDown.jqxRepeatButton({overrideTheme:true});this.btnDownInstance=a.data(this.btnDown[0],"jqxRepeatButton").instance;this.btnUpInstance=a.data(this.btnUp[0],"jqxRepeatButton").instance;this.areaUp.jqxRepeatButton({overrideTheme:true,delay:300});this.areaDown.jqxRepeatButton({overrideTheme:true,delay:300});this.btnThumb.jqxButton({overrideTheme:true});this.propertyChangeMap.value=function(d,f,e,g){d.setPosition(g)};this.propertyChangeMap.theme=function(d,f,e,g){d.setTheme()};this.propertyChangeMap.max=function(d,f,e,g){if(e!=g){d.setPosition(d.value);d._arrange()}};this.propertyChangeMap.min=function(d,f,e,g){if(e!=g){d.setPosition(d.value);d._arrange()}};this.propertyChangeMap.touchMode=function(d,f,e,g){if(e!=g){d._updateTouchBehavior()}};this.buttonUpCapture=false;this.buttonDownCapture=false;this._updateTouchBehavior();this.setPosition(this.value);this._addHandlers();this.setTheme()},_updateTouchBehavior:function(){this.isTouchDevice=a.jqx.mobile.isTouchDevice();if(this.touchMode==true){this.isTouchDevice=true;a.jqx.mobile.setMobileSimulator(this.btnThumb[0]);this._removeHandlers();this._addHandlers();this.setTheme()}else{if(this.touchMode==false){this.isTouchDevice=false}}},_addHandlers:function(){var d=this;if(d.isTouchDevice){this.addHandler(this.btnThumb,"touchend",function(f){var g=d.vertical?d.toThemeProperty("jqx-scrollbar-thumb-state-pressed"):d.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal");var h=d.toThemeProperty("jqx-fill-state-pressed");d.btnThumb.removeClass(g);d.btnThumb.removeClass(h);if(!d.disabled){d.handlemouseup(d,f)}});this.addHandler(this.btnThumb,"touchstart",function(f){if(!d.disabled){if(d.touchMode==true){f.clientX=f.originalEvent.clientX;f.clientY=f.originalEvent.clientY}else{var g=f;if(g.originalEvent.touches&&g.originalEvent.touches.length){f.clientX=g.originalEvent.touches[0].clientX;f.clientY=g.originalEvent.touches[0].clientY}}d.handlemousedown(f)}});a.jqx.mobile.touchScroll(this.element,d.max,function(l,k,g,f,h){if(d.host.css("visibility")=="visible"){if(d.touchMode==true){h.clientX=h.originalEvent.clientX;h.clientY=h.originalEvent.clientY}else{var j=h;if(j.originalEvent.touches&&j.originalEvent.touches.length){h.clientX=j.originalEvent.touches[0].clientX;h.clientY=j.originalEvent.touches[0].clientY}}var i=d.vertical?d.toThemeProperty("jqx-scrollbar-thumb-state-pressed"):d.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal");d.btnThumb.addClass(i);d.btnThumb.addClass(d.toThemeProperty("jqx-fill-state-pressed"));d.handlemousemove(h)}})}this.addHandler(this.btnUp,"click",function(f){if(d.buttonUpCapture&&!d.isTouchDevice){if(!d.disabled){d.setPosition(d.value-d.step)}}else{if(!d.disabled&&d.isTouchDevice){d.setPosition(d.value-d.step)}}});this.addHandler(this.btnDown,"click",function(f){if(d.buttonDownCapture&&!d.isTouchDevice){if(!d.disabled){d.setPosition(d.value+d.step)}}else{if(!d.disabled&&d.isTouchDevice){d.setPosition(d.value+d.step)}}});if(!this.isTouchDevice){if(window.frameElement){if(window.top!=null){var e=function(f){if(!d.disabled){d.handlemouseup(d,f)}};if(window.top.document.addEventListener){window.top.document.addEventListener("mouseup",e,false)}else{if(window.top.document.attachEvent){window.top.document.attachEvent("onmouseup",e)}}}}this.addHandler(this.btnDown,"mouseup",function(f){if(!d.btnDownInstance.base.disabled){d.buttonDownCapture=false;d.btnDown.removeClass(d.toThemeProperty("jqx-scrollbar-button-state-pressed"));d.btnDown.removeClass(d.toThemeProperty("jqx-fill-state-pressed"));d._removeArrowClasses("pressed","down");d.handlemouseup(d,f);d.setPosition(d.value+d.step);return false}});this.addHandler(this.btnUp,"mouseup",function(f){if(!d.btnUpInstance.base.disabled){d.buttonUpCapture=false;d.btnUp.removeClass(d.toThemeProperty("jqx-scrollbar-button-state-pressed"));d.btnUp.removeClass(d.toThemeProperty("jqx-fill-state-pressed"));d._removeArrowClasses("pressed","up");d.handlemouseup(d,f);d.setPosition(d.value-d.step);return false}});this.addHandler(this.btnDown,"mousedown",function(f){if(!d.btnDownInstance.base.disabled){d.buttonDownCapture=true;d.btnDown.addClass(d.toThemeProperty("jqx-fill-state-pressed"));d.btnDown.addClass(d.toThemeProperty("jqx-scrollbar-button-state-pressed"));d._addArrowClasses("pressed","down");return false}});this.addHandler(this.btnUp,"mousedown",function(f){if(!d.btnUpInstance.base.disabled){d.buttonUpCapture=true;d.btnUp.addClass(d.toThemeProperty("jqx-fill-state-pressed"));d.btnUp.addClass(d.toThemeProperty("jqx-scrollbar-button-state-pressed"));d._addArrowClasses("pressed","up");return false}})}var c="click";if(this.isTouchDevice){c="touchend"}this.addHandler(this.areaUp,c,function(f){if(!d.disabled){d.setPosition(d.value-d.largestep);return false}});this.addHandler(this.areaDown,c,function(f){if(!d.disabled){d.setPosition(d.value+d.largestep);return false}});this.addHandler(this.areaUp,"mousedown",function(f){if(!d.disabled){return false}});this.addHandler(this.areaDown,"mousedown",function(f){if(!d.disabled){return false}});this.addHandler(this.btnThumb,"mousedown",function(f){if(!d.disabled){d.handlemousedown(f)}return false});this.addHandler(this.btnThumb,"dragstart",function(f){return false});this.addHandler(a(document),"mouseup."+this.element.id,function(f){if(!d.disabled){d.handlemouseup(d,f)}});if(!this.isTouchDevice){this.addHandler(a(document),"mousemove."+this.element.id,function(f){if(!d.disabled){d.handlemousemove(f)}});this.addHandler(a(document),"mouseleave."+this.element.id,function(f){if(!d.disabled){d.handlemouseleave(f)}});this.addHandler(a(document),"mouseenter."+this.element.id,function(f){if(!d.disabled){d.handlemouseenter(f)}});if(!d.disabled){this.btnUp.hover(function(){if(!d.disabled&&!d.btnUpInstance.base.disabled&&d.touchMode!=true){d.btnUp.addClass(d.toThemeProperty("jqx-scrollbar-button-state-hover"));d.btnUp.addClass(d.toThemeProperty("jqx-fill-state-hover"));d._addArrowClasses("hover","up")}},function(){if(!d.disabled&&!d.btnUpInstance.base.disabled&&d.touchMode!=true){d.btnUp.removeClass(d.toThemeProperty("jqx-scrollbar-button-state-hover"));d.btnUp.removeClass(d.toThemeProperty("jqx-fill-state-hover"));d._removeArrowClasses("hover","up")}});var b=d.toThemeProperty("jqx-scrollbar-thumb-state-hover");if(!d.vertical){b=d.toThemeProperty("jqx-scrollbar-thumb-state-hover-horizontal")}this.btnThumb.hover(function(){if(!d.disabled&&d.touchMode!=true){d.btnThumb.addClass(b);d.btnThumb.addClass(d.toThemeProperty("jqx-fill-state-hover"))}},function(){if(!d.disabled&&d.touchMode!=true){d.btnThumb.removeClass(b);d.btnThumb.removeClass(d.toThemeProperty("jqx-fill-state-hover"))}});this.btnDown.hover(function(){if(!d.disabled&&!d.btnDownInstance.base.disabled&&d.touchMode!=true){d.btnDown.addClass(d.toThemeProperty("jqx-scrollbar-button-state-hover"));d.btnDown.addClass(d.toThemeProperty("jqx-fill-state-hover"));d._addArrowClasses("hover","down")}},function(){if(!d.disabled&&!d.btnDownInstance.base.disabled&&d.touchMode!=true){d.btnDown.removeClass(d.toThemeProperty("jqx-scrollbar-button-state-hover"));d.btnDown.removeClass(d.toThemeProperty("jqx-fill-state-hover"));d._removeArrowClasses("hover","down")}})}}},destroy:function(){var b=this.btnUp;var f=this.btnDown;var d=this.btnThumb;var c=this.scrollWrap;var g=this.areaUp;var e=this.areaDown;e.removeClass();g.removeClass();f.removeClass();b.removeClass();d.removeClass();b.jqxRepeatButton("destroy");f.jqxRepeatButton("destroy");g.jqxRepeatButton("destroy");e.jqxRepeatButton("destroy");d.jqxButton("destroy");this._removeHandlers();this.host.removeClass();this.host.remove()},_removeHandlers:function(){this.removeHandler(this.btnUp,"click");this.removeHandler(this.btnDown,"click");this.removeHandler(this.btnDown,"mouseup");this.removeHandler(this.btnUp,"mouseup");this.removeHandler(this.btnDown,"mousedown");this.removeHandler(this.btnUp,"mousedown");this.removeHandler(this.areaUp,"mousedown");this.removeHandler(this.areaDown,"mousedown");this.removeHandler(this.areaUp,"click");this.removeHandler(this.areaDown,"click");this.removeHandler(this.btnThumb,"mousedown");this.removeHandler(a(document),"mouseup."+this.element.id);this.removeHandler(a(document),"mousemove."+this.element.id);this.removeHandler(a(document),"mouseleave."+this.element.id);this.removeHandler(a(document),"mouseenter."+this.element.id);this.btnUp.unbind("hover");this.btnThumb.unbind("hover");this.btnDown.unbind("hover");var b=this},_addArrowClasses:function(c,b){if(c=="pressed"){c="selected"}if(c!=""){c="-"+c}if(this.vertical){if(b=="up"||b==undefined){this.arrowUp.addClass(this.toThemeProperty("icon-arrow-up"+c))}if(b=="down"||b==undefined){this.arrowDown.addClass(this.toThemeProperty("icon-arrow-down"+c))}}else{if(b=="up"||b==undefined){this.arrowUp.addClass(this.toThemeProperty("icon-arrow-left"+c))}if(b=="down"||b==undefined){this.arrowDown.addClass(this.toThemeProperty("icon-arrow-right"+c))}}},_removeArrowClasses:function(c,b){if(c=="pressed"){c="selected"}if(c!=""){c="-"+c}if(this.vertical){if(b=="up"||b==undefined){this.arrowUp.removeClass(this.toThemeProperty("icon-arrow-up"+c))}if(b=="down"||b==undefined){this.arrowDown.removeClass(this.toThemeProperty("icon-arrow-down"+c))}}else{if(b=="up"||b==undefined){this.arrowUp.removeClass(this.toThemeProperty("icon-arrow-left"+c))}if(b=="down"||b==undefined){this.arrowDown.removeClass(this.toThemeProperty("icon-arrow-right"+c))}}},setTheme:function(){var c=this.btnUp;var h=this.btnDown;var e=this.btnThumb;var d=this.scrollWrap;var i=this.areaUp;var g=this.areaDown;var f=this.arrowUp;var b=this.arrowDown;this.scrollWrap.removeClass();this.host.removeClass();f.removeClass();b.removeClass();g.removeClass();i.removeClass();h.removeClass();c.removeClass();e.removeClass();this.scrollWrap.removeClass();this.host.removeClass();this.scrollWrap.addClass(this.toThemeProperty("jqx-reset"));this.scrollOuterWrap.addClass(this.toThemeProperty("jqx-reset"));this.areaUp.addClass(this.toThemeProperty("jqx-reset"));this.areaDown.addClass(this.toThemeProperty("jqx-reset"));this.host.addClass(this.toThemeProperty("jqx-scrollbar"));this.host.addClass(this.toThemeProperty("jqx-widget"));this.host.addClass(this.toThemeProperty("jqx-widget-content"));h.addClass(this.toThemeProperty("jqx-scrollbar-button-state-normal"));c.addClass(this.toThemeProperty("jqx-scrollbar-button-state-normal"));if(this.vertical){f.addClass(this.toThemeProperty("icon-arrow-up"));b.addClass(this.toThemeProperty("icon-arrow-down"));e.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-normal"))}else{f.addClass(this.toThemeProperty("icon-arrow-left"));b.addClass(this.toThemeProperty("icon-arrow-right"));e.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-normal-horizontal"))}e.addClass(this.toThemeProperty("jqx-fill-state-normal"));if(this.disabled){d.addClass(this.toThemeProperty("jqx-fill-state-disabled"));d.removeClass(this.toThemeProperty("jqx-scrollbar-state-normal"))}else{d.addClass(this.toThemeProperty("jqx-scrollbar-state-normal"));d.removeClass(this.toThemeProperty("jqx-fill-state-disabled"))}if(c.jqxRepeatButton("disabled")!=this.disabled){c.jqxRepeatButton("disabled",this.disabled)}if(h.jqxRepeatButton("disabled")!=this.disabled){h.jqxRepeatButton("disabled",this.disabled)}if(e.jqxButton("disabled")!=this.disabled){e.jqxButton("disabled",this.disabled)}if(this.isTouchDevice&&this.touchModeStyle!=false){this.showButtons=false;e.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-normal-touch"))}},isScrolling:function(){if(this.thumbCapture==undefined||this.buttonDownCapture==undefined||this.buttonUpCapture==undefined){return false}return this.thumbCapture||this.buttonDownCapture||this.buttonUpCapture},handlemousedown:function(c){if(this.thumbCapture==undefined||this.thumbCapture==false){this.thumbCapture=true;var b=this.btnThumb;if(b!=null){b.addClass(this.toThemeProperty("jqx-fill-state-pressed"));if(this.vertical){b.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-pressed"))}else{b.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal"))}}}this.dragStartX=c.clientX;this.dragStartY=c.clientY;this.dragStartValue=this.value},toggleHover:function(c,b){},refresh:function(){this._arrange()},_setElementPosition:function(c,b,d){if(!isNaN(b)){c[0].style.left=b+"px"}if(!isNaN(d)){c[0].style.top=d+"px"}},_setElementTopPosition:function(b,c){if(!isNaN(c)){b[0].style.top=c+"px"}},_setElementLeftPosition:function(c,b){if(!isNaN(b)){c[0].style.left=b+"px"}},handlemouseleave:function(e){var b=this.btnUp;var d=this.btnDown;b.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));d.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));this._removeArrowClasses("pressed");if(this.thumbCapture!=true){return}var c=this.btnThumb;var f=this.vertical?this.toThemeProperty("jqx-scrollbar-thumb-state-pressed"):this.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal");c.removeClass(f);c.removeClass(this.toThemeProperty("jqx-fill-state-pressed"))},handlemouseenter:function(e){var b=this.btnUp;var d=this.btnDown;if(this.buttonUpCapture){b.addClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));b.addClass(this.toThemeProperty("jqx-fill-state-pressed"));this._addArrowClasses("pressed","up")}if(this.buttonDownCapture){d.addClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));d.addClass(this.toThemeProperty("jqx-fill-state-pressed"));this._addArrowClasses("pressed","down")}if(this.thumbCapture!=true){return}var c=this.btnThumb;if(this.vertical){c.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-pressed"))}else{c.addClass(this.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal"))}c.addClass(this.toThemeProperty("jqx-fill-state-pressed"))},handlemousemove:function(b){var h=this.btnUp;var d=this.btnDown;var c=0;if(d==null||h==null){return}if(h!=null&&d!=null&&this.buttonDownCapture!=undefined&&this.buttonUpCapture!=undefined){if(this.buttonDownCapture&&b.which==c){d.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));d.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));this._removeArrowClasses("pressed","down");this.buttonDownCapture=false}else{if(this.buttonUpCapture&&b.which==c){h.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));h.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));this._removeArrowClasses("pressed","up");this.buttonUpCapture=false}}}if(this.thumbCapture!=true){return false}var j=this.btnThumb;if(b.which==c&&!this.isTouchDevice){this.thumbCapture=false;this._arrange();var i=this.vertical?this.toThemeProperty("jqx-scrollbar-thumb-state-pressed"):this.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal");j.removeClass(i);j.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));return true}if(b.preventDefault!=undefined){b.preventDefault()}if(b.originalEvent!=null){b.originalEvent.mouseHandled=true}if(b.stopPropagation!=undefined){b.stopPropagation()}var k=0;try{if(!this.vertical){k=b.clientX-this.dragStartX}else{k=b.clientY-this.dragStartY}var e=this._btnAndThumbSize;if(!this._btnAndThumbSize){e=(this.vertical)?h.height()+d.height()+j.height():h.width()+d.width()+j.width()}var f=(this.max-this.min)/(this.scrollBarSize-e);k*=f;this.setPosition(this.dragStartValue+k)}catch(g){alert(g)}return false},handlemouseup:function(d,g){var c=false;if(this.thumbCapture){this.thumbCapture=false;var e=this.btnThumb;var h=this.vertical?this.toThemeProperty("jqx-scrollbar-thumb-state-pressed"):this.toThemeProperty("jqx-scrollbar-thumb-state-pressed-horizontal");e.removeClass(h);e.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));c=true}if(this.buttonUpCapture||this.buttonDownCapture){var b=this.btnUp;var f=this.btnDown;this.buttonUpCapture=false;this.buttonDownCapture=false;b.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));f.removeClass(this.toThemeProperty("jqx-scrollbar-button-state-pressed"));b.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));f.removeClass(this.toThemeProperty("jqx-fill-state-pressed"));this._removeArrowClasses("pressed");c=true}if(c){if(g.preventDefault!=undefined){g.preventDefault()}if(g.originalEvent!=null){g.originalEvent.mouseHandled=true}if(g.stopPropagation!=undefined){g.stopPropagation()}}},setPosition:function(b){var d=this.element;if(b==undefined||b==NaN){b=this.min}if(b>=this.max){b=this.max}if(b<this.min){b=this.min}var e=new jQuery.Event("valuechanged");if(this.value!=b){if(b==this.max){var c=new jQuery.Event("complete");this.host.trigger(c)}e.previousValue=this.value;e.currentValue=b;this.value=b;this._positionelements();if(this._triggervaluechanged){this.host.trigger(e)}if(this.valuechanged){this.valuechanged({currentValue:e.currentValue,previousvalue:e.previousValue})}}return b},_getThumbSize:function(b){var d=this.max-this.min;var c=0;if(d>1){c=(b/(d+b)*b)}else{if(d==1){c=b}}if(this.thumbSize>0){c=this.thumbSize}if(c<this.thumbMinSize){c=this.thumbMinSize}return Math.min(c,b)},_positionelements:function(){var g=this.element;var n=this.areaUp;var e=this.areaDown;var h=this.btnUp;var f=this.btnDown;var o=this.btnThumb;var b=this.scrollWrap;var p=this._height?this._height:this.host.height();var c=this._width?this._width:this.host.width();var l=(!this.vertical)?p:c;if(!this.showButtons){l=0}var m=(!this.vertical)?c:p;this.scrollBarSize=m;var d=this._getThumbSize(m-2*l);d=Math.round(d);if(d<this.thumbMinSize){d=this.thumbMinSize}if(p==NaN||p<10){p=10}if(c==NaN||c<10){c=10}l+=2;this.btnSize=l;var i=this._btnAndThumbSize;if(!this._btnAndThumbSize){var i=(this.vertical)?2*this.btnSize+o.outerHeight():2*this.btnSize+o.outerWidth();i=Math.round(i)}var k=(m-i)/(this.max-this.min)*this.value;k=Math.round(k);if(this.vertical){var j=m-k-i;if(j<0){j=0}e[0].style.height=j+"px";n[0].style.height=k+"px";this._setElementTopPosition(n,l);this._setElementTopPosition(o,l+k);this._setElementTopPosition(e,l+k+d)}else{n[0].style.width=k+"px";e[0].style.width=m-k-i+"px";this._setElementLeftPosition(n,l);this._setElementLeftPosition(o,l+k);this._setElementLeftPosition(e,2+l+k+d)}},_arrange:function(){var d=this.element;var h=this.areaUp;var u=this.areaDown;var c=this.btnUp;var m=this.btnDown;var v=this.btnThumb;var p=this.scrollWrap;if(this.roundedCorners=="all"){if(this.vertical){c.jqxRepeatButton();m.jqxRepeatButton();var g=a.jqx.cssroundedcorners("top");g=this.toThemeProperty(g);c.addClass(g);var r=a.jqx.cssroundedcorners("bottom");r=this.toThemeProperty(r);m.addClass(r)}else{c.jqxRepeatButton();m.jqxRepeatButton();var l=a.jqx.cssroundedcorners("left");l=this.toThemeProperty(l);c.addClass(l);var i=a.jqx.cssroundedcorners("right");i=this.toThemeProperty(i);m.addClass(i)}}else{var t=a.jqx.cssroundedcorners(this.roundedCorners);t=this.toThemeProperty(t);c.addClass(t);m.addClass(t)}v.jqxButton();var t=a.jqx.cssroundedcorners(this.roundedCorners);t=this.toThemeProperty(t);v.addClass(t);var n=this.host.height();var q=this.host.width();this._width=q;this._height=n;var b=(!this.vertical)?n:q;if(!this.showButtons){b=0}c.css({width:b+"px"});c.css({height:b+"px"});m.css({width:b+"px"});m.css({height:b+"px"});if(this.vertical){p.css({width:q+2+"px"})}else{p.css({height:n+2+"px"})}this._setElementPosition(c,0,0);if(this.vertical){this._setElementPosition(m,0,n-m.outerHeight())}else{this._setElementPosition(m,q-m.outerWidth(),0)}var f=(!this.vertical)?q:n;this.scrollBarSize=f;var j=this._getThumbSize(f-2*b);j=Math.round(j);if(j<this.thumbMinSize){j=this.thumbMinSize}var o=false;if(this.isTouchDevice&&this.touchModeStyle!=false){o=true}if(!this.vertical){v.css({width:j+"px"});v.css({height:n+"px"});if(o){v.css({height:this.thumbTouchSize+"px"});v.css("margin-top",(this.host.height()-this.thumbTouchSize)/2)}}else{v.css({width:q+"px"});v.css({height:j+"px"});if(o){v.css({width:this.thumbTouchSize+"px"});v.css("margin-left",(this.host.width()-this.thumbTouchSize)/2)}}if(n==NaN||n<10){n=10}if(q==NaN||q<10){q=10}b+=2;this.btnSize=b;var e=(this.vertical)?2*this.btnSize+v.outerHeight():2*this.btnSize+v.outerWidth();e=Math.round(e);this._btnAndThumbSize=e;var w=(f-e)/(this.max-this.min)*this.value;w=Math.round(w);if(isNaN(w)){w=0}if(this.vertical){u.css({height:(f-w-e)+"px",width:q+"px"});h.css({height:w+"px",width:q+"px"});var k=parseInt(this.host.height());v.css("visibility","inherit");if(k-3*parseInt(b)<0){v.css("visibility","hidden")}else{if(k<e){v.css("visibility","hidden")}else{if(this.host.css("visibility")=="visible"){v.css("visibility","inherit")}}}this._setElementPosition(h,0,b);this._setElementPosition(v,0,b+w);this._setElementPosition(u,0,b+w+j)}else{h.css({width:w+"px",height:n+"px"});u.css({width:(f-w-e)+"px",height:n+"px"});var s=parseInt(this.host.width());v.css("visibility","inherit");if(s-3*parseInt(b)<0){v.css("visibility","hidden")}else{if(s<e){v.css("visibility","hidden")}else{if(this.host.css("visibility")=="visible"){v.css("visibility","inherit")}}}this._setElementPosition(h,b,0);this._setElementPosition(v,b+w,0);this._setElementPosition(u,2+b+w+j,0)}}})})(jQuery);